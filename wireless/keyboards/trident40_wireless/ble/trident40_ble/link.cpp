// Trident40 Wireless HY0020: framed UART link to the STM32.
//
// The link uses UARTE0 directly, not the core's Serial: Serial receives one byte per EasyDMA
// transfer and re-arms the next one in its interrupt, so every interrupt that comes late (the
// SoftDevice, flash writes) loses bytes (~2 % of the frames at 250 kbaud, 2026-10-30).
//
// Here the receiver never stops: one 255-byte buffer that the ENDRX -> STARTRX shortcut restarts
// in hardware, so it works as a ring, and TIMER1 counts the received bytes (RXDRDY -> COUNT
// through PPI). The ring holds ~10 ms of data. loop() can block for longer than that (Bluefruit's
// notify() waits for a free notification slot, ~10 % of the frames were lost when loop() read the
// ring), so a task of its own moves the bytes into a 2 KB buffer every millisecond while the
// receiver is on, and wakes loop() for them.
// While the receiver is off, the STM32's wake byte (RX low) is seen through the pin's SENSE: the
// GPIO DETECT signal makes a GPIOTE PORT event, which PPI turns into an EGU3 interrupt. A GPIOTE IN
// event (attachInterrupt) would keep a high-frequency clock running all the time (~20 uA); the PORT
// event costs nothing while it waits. The core owns GPIOTE's interrupt handler, hence the EGU.
// PPI goes through the SoftDevice API: link_begin() must run after Bluefruit.begin().
#include <bluefruit.h>
#include "link.h"
#include "config.h"
#include "wake.h"
#include "stats.h"

#define LINK_UARTE       NRF_UARTE0
#define LINK_TIMER       NRF_TIMER1        // TIMER0: SoftDevice, TIMER2 CC[0]: bootloader version
#define LINK_PPI_CH      7                 // application PPI channels
#define LINK_WAKE_PPI_CH 8
#define LINK_WAKE_EGU    NRF_EGU3          // SWI3/EGU3: not used by the SoftDevice or the core
#define DMA_RING_LEN     255               // UARTE MAXCNT is 8 bits on the nRF52832
#define RX_RING_LEN      2048              // power of two

static_assert(LINK_BAUD == 250000UL, "uart_start() sets the UARTE to 250 kbaud");

static uint8_t crc8_update(uint8_t crc, uint8_t b) {
  crc ^= b;
  for (int i = 0; i < 8; i++) crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x07) : (uint8_t)(crc << 1);
  return crc;
}

// Byte i of the received stream is dma_ring[i % DMA_RING_LEN] until the next 255 bytes overwrite it.
static uint8_t dma_ring[DMA_RING_LEN];
static uint32_t dma_taken;                 // stream bytes moved to rx_ring (rx task)
static uint32_t dma_count_prev;            // count at the previous pass: those bytes are in RAM by now
// Filled by the rx task, read by loop().
static uint8_t rx_ring[RX_RING_LEN];
static volatile uint32_t rx_head, rx_tail;
static volatile uint16_t rx_overruns;

static uint8_t tx_buf[LINK_MAX_PAYLOAD + 4];

static volatile bool uart_on;
static volatile bool rx_edge;
static volatile uint32_t last_rx_ms;
static TaskHandle_t rx_task;

static uint32_t dma_count() {
  LINK_TIMER->TASKS_CAPTURE[0] = 1;
  return LINK_TIMER->CC[0];
}

// rx task: every millisecond while the receiver is on, else asleep until uart_start().
static void rx_task_fn(void *) {
  for (;;) {
    if (!uart_on) {
      ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
      continue;
    }
    uint32_t count = dma_count();
    uint32_t ready = dma_count_prev;       // RXDRDY can come before the byte's DMA write
    dma_count_prev = count;
    if (ready - dma_taken > DMA_RING_LEN) {   // more than a ring behind: that data is gone
      dma_taken = ready;
      rx_overruns++;
    }
    bool got = false;
    while (dma_taken != ready && rx_head - rx_tail < RX_RING_LEN) {
      rx_ring[rx_head % RX_RING_LEN] = dma_ring[dma_taken % DMA_RING_LEN];
      dma_taken++;
      rx_head++;
      got = true;
    }
    if (count != ready || got) last_rx_ms = millis();
    if (got) loop_wake();
    vTaskDelay(1);
  }
}

extern "C" void SWI3_EGU3_IRQHandler(void) {
  LINK_WAKE_EGU->EVENTS_TRIGGERED[0] = 0;
  NRF_GPIOTE->EVENTS_PORT = 0;
  rx_edge = true;
  loop_wake_from_isr();
}

static void rx_sense(bool on) {
  nrf_gpio_cfg_sense_input(PIN_LINK_RX, NRF_GPIO_PIN_PULLUP, on ? NRF_GPIO_PIN_SENSE_LOW : NRF_GPIO_PIN_NOSENSE);
}

static void uart_start() {
  if (uart_on) return;
  rx_sense(false);
  digitalWrite(PIN_LINK_TX, HIGH);
  pinMode(PIN_LINK_TX, OUTPUT);

  LINK_TIMER->TASKS_STOP = 1;
  LINK_TIMER->MODE = TIMER_MODE_MODE_Counter;
  LINK_TIMER->BITMODE = TIMER_BITMODE_BITMODE_32Bit;
  LINK_TIMER->TASKS_CLEAR = 1;
  LINK_TIMER->TASKS_START = 1;
  sd_ppi_channel_assign(LINK_PPI_CH, &LINK_UARTE->EVENTS_RXDRDY, &LINK_TIMER->TASKS_COUNT);
  sd_ppi_channel_enable_set(1UL << LINK_PPI_CH);

  LINK_UARTE->INTENCLR = 0xFFFFFFFF;       // polled; the core's Serial owns the IRQ handler
  LINK_UARTE->PSEL.TXD = PIN_LINK_TX;
  LINK_UARTE->PSEL.RXD = PIN_LINK_RX;
  LINK_UARTE->CONFIG = UARTE_CONFIG_HWFC_Disabled << UARTE_CONFIG_HWFC_Pos;
  LINK_UARTE->BAUDRATE = UARTE_BAUDRATE_BAUDRATE_Baud250000;
  LINK_UARTE->ENABLE = UARTE_ENABLE_ENABLE_Enabled;

  dma_taken = dma_count_prev = 0;
  LINK_UARTE->EVENTS_ENDRX = 0;
  LINK_UARTE->EVENTS_RXTO = 0;
  LINK_UARTE->SHORTS = UARTE_SHORTS_ENDRX_STARTRX_Msk;
  LINK_UARTE->RXD.PTR = (uint32_t)dma_ring;
  LINK_UARTE->RXD.MAXCNT = DMA_RING_LEN;
  LINK_UARTE->TASKS_STARTRX = 1;

  last_rx_ms = millis();
  uart_on = true;
  nrf_stats.uart_starts++;
  nrf_stats.uart_on_since = millis();
  xTaskNotifyGive(rx_task);
}

// Both directions have to stop, or the UARTE keeps its transceiver powered (~0.9 mA).
static void uarte_off() {
  if (uart_on) nrf_stats.uart_on_ms += millis() - nrf_stats.uart_on_since;
  uart_on = false;
  LINK_UARTE->SHORTS = 0;
  LINK_UARTE->EVENTS_RXTO = 0;
  LINK_UARTE->TASKS_STOPRX = 1;
  LINK_UARTE->TASKS_STOPTX = 1;
  uint32_t t = millis();
  while (!LINK_UARTE->EVENTS_RXTO && millis() - t < 5) {}
  LINK_UARTE->ENABLE = UARTE_ENABLE_ENABLE_Disabled;
  LINK_UARTE->PSEL.TXD = 0xFFFFFFFF;
  LINK_UARTE->PSEL.RXD = 0xFFFFFFFF;
  sd_ppi_channel_enable_clr(1UL << LINK_PPI_CH);
  LINK_TIMER->TASKS_STOP = 1;
  LINK_TIMER->TASKS_SHUTDOWN = 1;
}

static void uart_stop() {
  if (!uart_on) return;
  uarte_off();
  rx_edge = false;
  pinMode(PIN_LINK_TX, OUTPUT);                    // idle high
  digitalWrite(PIN_LINK_TX, HIGH);
  rx_sense(true);                                  // RX already low: the event comes at once
}

void link_begin() {
  pinMode(PIN_IRQ, OUTPUT);
  digitalWrite(PIN_IRQ, HIGH);
  LINK_WAKE_EGU->EVENTS_TRIGGERED[0] = 0;
  LINK_WAKE_EGU->INTENSET = EGU_INTENSET_TRIGGERED0_Msk;
  NVIC_SetPriority(SWI3_EGU3_IRQn, 3);            // as the core's GPIOTE: may use FreeRTOS FromISR calls
  NVIC_EnableIRQ(SWI3_EGU3_IRQn);
  sd_ppi_channel_assign(LINK_WAKE_PPI_CH, &NRF_GPIOTE->EVENTS_PORT, &LINK_WAKE_EGU->TASKS_TRIGGER[0]);
  sd_ppi_channel_enable_set(1UL << LINK_WAKE_PPI_CH);
  // above loop() (TASK_PRIO_LOW), so a blocked loop() does not stop the reading
  xTaskCreate(rx_task_fn, "link_rx", 256, nullptr, TASK_PRIO_NORMAL, &rx_task);
  uart_start();
}

bool link_uart_on() { return uart_on; }
TaskHandle_t link_rx_task() { return rx_task; }

void link_idle_task() {
  if (uart_on && rx_head == rx_tail && millis() - last_rx_ms > LINK_UART_IDLE_MS) uart_stop();
}

void link_system_off_prepare() {
  if (uart_on) uarte_off();
  pinMode(PIN_LINK_TX, OUTPUT);
  digitalWrite(PIN_LINK_TX, HIGH);
  // RX low (the STM32's wake byte) starts the chip again; the pin configuration stays in System OFF.
  nrf_gpio_cfg_sense_input(PIN_LINK_RX, NRF_GPIO_PIN_PULLUP, NRF_GPIO_PIN_SENSE_LOW);
}

// Frames to the STM32: IRQ low first (it may be in Stop), the frame LINK_IRQ_LEAD_US later, IRQ
// high again when the last byte is out.
void link_send(uint8_t type, const uint8_t *payload, uint8_t len) {
  if (len > LINK_MAX_PAYLOAD) return;
  bool was_on = uart_on;
  uart_start();
  uint8_t n = 0, crc = 0;
  tx_buf[n++] = LINK_SOF;
  tx_buf[n++] = type;
  tx_buf[n++] = len;
  if (len) memcpy(tx_buf + n, payload, len), n += len;
  for (uint8_t i = 1; i < n; i++) crc = crc8_update(crc, tx_buf[i]);
  tx_buf[n++] = crc;
  digitalWrite(PIN_IRQ, LOW);
  delayMicroseconds(LINK_IRQ_LEAD_US);
  LINK_UARTE->EVENTS_ENDTX = 0;
  LINK_UARTE->TXD.PTR = (uint32_t)tx_buf;
  LINK_UARTE->TXD.MAXCNT = n;
  LINK_UARTE->TASKS_STARTTX = 1;
  uint32_t t = millis();
  while (!LINK_UARTE->EVENTS_ENDTX && millis() - t < 5) {}
  LINK_UARTE->EVENTS_ENDTX = 0;
  digitalWrite(PIN_IRQ, HIGH);
  if (!was_on) last_rx_ms = millis() - LINK_UART_IDLE_MS + 50;   // answer window, then off again
}

enum { RX_SOF, RX_TYPE, RX_LEN, RX_DATA, RX_CRC };
static uint8_t rx_state = RX_SOF, rx_pos, rx_crc;
static uint32_t rx_byte_ms;
// The frame being received lives here: it may span several loop() passes.
static LinkFrame rx;
static uint16_t rx_frames, rx_dropped;
static uint16_t overruns_seen;

uint16_t link_rx_frames() { return rx_frames; }
uint16_t link_rx_dropped() { return rx_dropped; }

bool link_poll(LinkFrame &f) {
  if (!uart_on) {
    if (rx_edge) {
      rx_edge = false;
      uart_start();
    }
    if (rx_head == rx_tail) return false;
  }
  if (rx_overruns != overruns_seen) {             // bytes were lost: whatever frame was open is broken
    rx_dropped += (uint16_t)(rx_overruns - overruns_seen);
    overruns_seen = rx_overruns;
    rx_state = RX_SOF;
  }
  // A gap inside a frame ends it (the rx task moves bytes every millisecond).
  if (rx_state != RX_SOF && rx_head == rx_tail && millis() - rx_byte_ms > LINK_BYTE_TIMEOUT_MS) {
    rx_state = RX_SOF;
    rx_dropped++;
  }
  while (rx_tail != rx_head) {
    uint8_t c = rx_ring[rx_tail % RX_RING_LEN];
    rx_tail++;
    rx_byte_ms = millis();
    switch (rx_state) {
      case RX_SOF:
        if (c == LINK_SOF) { rx_state = RX_TYPE; rx_crc = 0; }
        break;
      case RX_TYPE:
        rx.type = c; rx_crc = crc8_update(rx_crc, c); rx_state = RX_LEN;
        break;
      case RX_LEN:
        if (c > LINK_MAX_PAYLOAD) { rx_state = RX_SOF; rx_dropped++; break; }
        rx.len = c; rx_crc = crc8_update(rx_crc, c); rx_pos = 0;
        rx_state = c ? RX_DATA : RX_CRC;
        break;
      case RX_DATA:
        rx.data[rx_pos++] = c; rx_crc = crc8_update(rx_crc, c);
        if (rx_pos == rx.len) rx_state = RX_CRC;
        break;
      case RX_CRC:
        rx_state = RX_SOF;
        if (c != rx_crc) {
          rx_dropped++;
        } else {
          rx_frames++;
          f.type = rx.type;
          f.len = rx.len;
          memcpy(f.data, rx.data, rx.len);
          return true;
        }
        break;
    }
  }
  return false;
}
