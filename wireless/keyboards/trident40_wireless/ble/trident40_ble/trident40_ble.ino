// Trident40 Wireless HY0020 (nRF52832): BLE side of the keyboard.
// The STM32 (QMK / Vial) sends HID reports over the UART link (link_proto.h); this firmware is the
// BLE HID device for them, relays Vial's raw HID both ways and reports the BLE state back.
// Board: Adafruit nRF52 core, Feather nRF52832 (the HY0020 carries the Adafruit bootloader and
// SoftDevice S132 6.1.1).
#include <bluefruit.h>
#include "config.h"
#include "link.h"
#include "ble.h"
#include "wake.h"
#include "stats.h"
#include "fault.h"

NrfStats nrf_stats;

static void wdt_start() {
  if (NRF_WDT->RUNSTATUS) return;              // survives a soft reset: already configured
  NRF_WDT->CONFIG = (WDT_CONFIG_HALT_Pause << WDT_CONFIG_HALT_Pos) | (WDT_CONFIG_SLEEP_Run << WDT_CONFIG_SLEEP_Pos);
  NRF_WDT->CRV = (uint32_t)NRF_WDT_TIMEOUT_MS * 32768UL / 1000UL;
  NRF_WDT->RREN = WDT_RREN_RR0_Msk;
  NRF_WDT->TASKS_START = 1;
}

// ---- reports from the STM32 ----------------------------------------------------------------
// Keyboard reports go out in order: several can arrive in one loop() pass (send_string, fast
// typing), and keeping only the latest would drop the presses and releases in between. When the
// SoftDevice queue is full they wait here. The other kinds keep only the latest (a newer one
// replaces an older one that never went out).
#define KB_QUEUE_LEN 256
#define KB_SEND_PER_PASS 4
static uint8_t kb_queue[KB_QUEUE_LEN][8];
static uint16_t kb_head, kb_count;
static uint8_t mouse_report[5];
static uint16_t consumer_usage, system_usage;
static uint8_t vial_report[32];
enum { P_CONSUMER = 2, P_SYSTEM = 4, P_MOUSE = 8, P_VIAL = 16 };
static uint8_t pending;

static void kb_push(const uint8_t *r) {
  if (kb_count == KB_QUEUE_LEN) {              // full: the newest state replaces the last one queued
    memcpy(kb_queue[(kb_head + kb_count - 1) % KB_QUEUE_LEN], r, 8);
    return;
  }
  memcpy(kb_queue[(kb_head + kb_count) % KB_QUEUE_LEN], r, 8);
  kb_count++;
}

static void send_pending() {
  if (!pending && !kb_count) return;
  if (!ble_secured()) {
    pending = 0;                               // nobody to send to: dropped (the host gets a fresh state on connect)
    kb_count = 0;
    return;
  }
  // A few per pass: loop() must keep reading the link (link.cpp) while BLE catches up.
  for (uint8_t n = 0; n < KB_SEND_PER_PASS && kb_count && ble_keyboard(kb_queue[kb_head]); n++) {
    kb_head = (kb_head + 1) % KB_QUEUE_LEN;
    kb_count--;
  }
  if ((pending & P_CONSUMER) && ble_consumer(consumer_usage)) pending &= ~P_CONSUMER;
  if ((pending & P_SYSTEM) && ble_system((uint8_t)system_usage)) pending &= ~P_SYSTEM;
  if ((pending & P_MOUSE) && ble_mouse(mouse_report)) pending &= ~P_MOUSE;
  if ((pending & P_VIAL) && ble_vial_input(vial_report, 32)) pending &= ~P_VIAL;
}

static uint8_t bt_state() {
  if (ble_secured()) return BT_STATE_CONNECTED;
  if (ble_advertising()) return BT_STATE_ADVERTISING;
  return BT_STATE_IDLE;
}

static void send_status() {
  uint16_t rx = link_rx_frames(), dropped = link_rx_dropped(), interval, latency, timeout;
  ble_conn_params(interval, latency, timeout);
  uint8_t s[11] = {bt_state(), (uint8_t)rx, (uint8_t)(rx >> 8), (uint8_t)dropped, (uint8_t)(dropped >> 8),
                   (uint8_t)interval, (uint8_t)(interval >> 8), (uint8_t)latency, (uint8_t)(latency >> 8),
                   (uint8_t)timeout, (uint8_t)(timeout >> 8)};
  link_send(MSG_STATUS, s, sizeof(s));
}

// Counters since the start (stats.h). Open spans (receiver on, latency off) count up to now.
static void send_stats() {
  static uint8_t reset_reason;
  static bool reset_read;
  if (!reset_read) {
    uint32_t r = readResetReason();               // RESETREAS at start-up
    reset_reason = (r & 0x0F) | (r & POWER_RESETREAS_OFF_Msk ? 0x10 : 0);
    reset_read = true;
  }
  uint32_t now = millis();
  uint32_t uart_ms = nrf_stats.uart_on_ms + (link_uart_on() ? now - nrf_stats.uart_on_since : 0);
  uint32_t lat_ms = nrf_stats.latency_off_ms + (nrf_stats.latency_off ? now - nrf_stats.latency_off_since : 0);
  uint32_t hfclk = 0;
  sd_clock_hfclk_is_running(&hfclk);
  uint8_t flags = (link_uart_on() ? 1 : 0) | (nrf_stats.latency_off ? 2 : 0) | (hfclk ? 4 : 0) |
                  (NRF_POWER->DCDCEN ? 8 : 0);
  uint32_t v32[6] = {now / 1000, nrf_stats.loop_wakes, nrf_stats.busy_ms, uart_ms, nrf_stats.uart_starts, lat_ms};
  uint8_t s[30];
  memcpy(s, v32, 24);                              // little-endian like the rest of the link
  memcpy(s + 24, &nrf_stats.connects, 2);
  memcpy(s + 26, &nrf_stats.param_changes, 2);
  s[28] = flags;
  s[29] = reset_reason;
  link_send(MSG_STATS, s, sizeof(s));
}

// The STM32 sends KEYBOARD, CONSUMER and SYSTEM twice (link_proto.h): drop the repeat.
static bool repeat_of_last(uint8_t type, const uint8_t *data, uint8_t len) {
  static uint8_t last[3][8];
  static bool seen[3];
  uint8_t i = type - MSG_KEYBOARD;                 // MSG_KEYBOARD, MSG_CONSUMER, MSG_SYSTEM
  if (seen[i] && memcmp(last[i], data, len) == 0) return true;
  memcpy(last[i], data, len);
  seen[i] = true;
  return false;
}

static void handle_frame(const LinkFrame &f) {
  switch (f.type) {
    case MSG_KEYBOARD:
      if (f.len != 8 || repeat_of_last(f.type, f.data, 8)) break;
      kb_push(f.data);
      break;
    case MSG_CONSUMER:
      if (f.len != 2 || repeat_of_last(f.type, f.data, 2)) break;
      consumer_usage = f.data[0] | (f.data[1] << 8);
      pending |= P_CONSUMER;
      break;
    case MSG_SYSTEM:
      if (f.len != 2 || repeat_of_last(f.type, f.data, 2)) break;
      system_usage = f.data[0] | (f.data[1] << 8);
      pending |= P_SYSTEM;
      break;
    case MSG_MOUSE:
      if (f.len != 5) break;
      if ((pending & P_MOUSE) && mouse_report[0] == f.data[0]) {
        // the last one has not gone out: add the motion instead of losing it
        for (uint8_t i = 1; i < 5; i++) {
          int v = (int8_t)mouse_report[i] + (int8_t)f.data[i];
          mouse_report[i] = (uint8_t)(int8_t)(v < -127 ? -127 : v > 127 ? 127 : v);
        }
      } else {
        memcpy(mouse_report, f.data, 5);
      }
      pending |= P_MOUSE;
      break;
    case MSG_VIAL_IN:
      if (f.len != 32) break;
      memcpy(vial_report, f.data, 32);
      pending |= P_VIAL;
      break;
    case MSG_BATTERY:
      if (f.len >= 1) ble_battery(f.data[0]);
      break;
    case MSG_BT_CMD:
      if (f.len < 1) break;
      if (f.data[0] == BT_CMD_ADVERTISE && !ble_connected()) ble_start_advertising();
      if (f.data[0] == BT_CMD_ERASE_ALL) ble_erase_all();   // does not return
      if (f.data[0] == BT_CMD_TEST_STALL) for (;;) {}       // until the watchdog (fault.h)
      if (f.data[0] == BT_CMD_ENTER_DFU) {
        enterSerialDfu();                          // does not return
      }
      break;
    case MSG_PING:
      send_status();
      break;
    case MSG_STATS_REQ:
      send_stats();
      break;
  }
}

// ---- reset pin ------------------------------------------------------------------------------
// P0.21 works as the reset pin only when UICR PSELRESET says so. The bootloader on the AE-HY0020-DIP
// sets it on every boot; on bare HY0020 modules it was left erased (the reset pin and the DTR
// auto-reset did nothing, seen 2026-10-29). An erased value is written once (bits 1 -> 0 need no
// erase) and the chip restarts for it to take effect; any other value is left alone. Runs before
// the SoftDevice is enabled, so the NVMC can be used directly.
#define RESET_PIN 21

static bool reset_pin_ok() {
  return NRF_UICR->PSELRESET[0] == RESET_PIN && NRF_UICR->PSELRESET[1] == RESET_PIN;
}

static void nvmc_wait() { while (!NRF_NVMC->READY) {} }

static void reset_pin_enable() {
  if (reset_pin_ok()) return;
  if (NRF_UICR->PSELRESET[0] != 0xFFFFFFFF || NRF_UICR->PSELRESET[1] != 0xFFFFFFFF) return;
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Wen;
  nvmc_wait();
  NRF_UICR->PSELRESET[0] = RESET_PIN;
  nvmc_wait();
  NRF_UICR->PSELRESET[1] = RESET_PIN;
  nvmc_wait();
  NRF_NVMC->CONFIG = NVMC_CONFIG_WEN_Ren;
  nvmc_wait();
  NVIC_SystemReset();
}

// ---- resting battery sample ------------------------------------------------------------------
// The STM32 has no clock in Stop, so the nRF says when: MSG_BAT_SAMPLE (link_proto.h) once the link
// has been quiet for a while, then at most every LINK_BAT_SAMPLE_PERIOD_S.
static void bat_sample_task() {
  static uint32_t quiet_ms, sent_ms;
  static bool sent;
  uint32_t now = millis();
  // Connected only: link_send() starts the receiver, which would keep System OFF away.
  if (link_uart_on() || !ble_connected()) { quiet_ms = now; return; }
  if (now - quiet_ms < LINK_BAT_SAMPLE_QUIET_S * 1000UL) return;
  if (sent && now - sent_ms < LINK_BAT_SAMPLE_PERIOD_S * 1000UL) return;
  sent = true;
  sent_ms = now;
  link_send(MSG_BAT_SAMPLE, nullptr, 0);
}

// ---- System OFF ----------------------------------------------------------------------------
static void system_off_task() {
  static uint32_t active_ms;                       // last connection or link traffic
  if (ble_connected() || link_uart_on()) active_ms = millis();
  if (millis() - active_ms < SYSTEM_OFF_IDLE_MS) return;
  Bluefruit.Advertising.stop();
  link_system_off_prepare();
  sd_power_system_off();
}

void setup() {
  reset_pin_enable();
  loop_wake_init();
  ble_begin();
  link_begin();                                  // after the SoftDevice is up (link.cpp uses its PPI API)
  const uint8_t hello[5] = {LINK_PROTO_VERSION, FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH, reset_pin_ok()};
  link_send(MSG_HELLO, hello, sizeof(hello));
  send_status();
  fault_begin(xTaskGetCurrentTaskHandle(), link_rx_task());
  wdt_start();
}

void loop() {
  static uint8_t last_state = 0xFF;
  static uint32_t pass_ms;
  static bool was_busy;
  fault_alive();
  uint32_t t = millis();
  if (was_busy) nrf_stats.busy_ms += t - pass_ms;
  pass_ms = t;
  nrf_stats.loop_wakes++;

  LinkFrame f;
  fault_stage = STAGE_LINK_POLL;
  while (link_poll(f)) {
    fault_stage = STAGE_FRAME;
    fault_frame_type = f.type;
    fault_frame_arg = f.len ? f.data[0] : 0;
    uint32_t t0 = millis();
    handle_frame(f);
    if (millis() - t0 >= FAULT_SLOW_MS) fault_slow(millis() - t0);
    fault_stage = STAGE_LINK_POLL;
  }
  fault_stage = STAGE_SEND_PENDING;
  send_pending();

  fault_stage = STAGE_TO_STM32;
  uint8_t leds;
  if (ble_take_leds(leds)) link_send(MSG_LEDS, &leds, 1);
  uint8_t vial[32];
  while (ble_take_vial_output(vial)) link_send(MSG_VIAL_OUT, vial, 32);

  fault_stage = STAGE_STATUS;
  uint8_t state = bt_state();
  if (state != last_state) {
    last_state = state;
    send_status();
  }

  fault_stage = STAGE_CONN;
  ble_conn_task();
  fault_stage = STAGE_LINK_IDLE;
  link_idle_task();
  fault_stage = STAGE_BAT_SAMPLE;
  bat_sample_task();
  fault_stage = STAGE_SYSTEM_OFF;
  system_off_task();
  fault_stage = STAGE_WAIT;
  // Reports waiting for BLE: come back in LOOP_BUSY_MS. Else sleep until link bytes (link.cpp's rx
  // task or the RX edge), a BLE event or LOOP_UART_MS / LOOP_SLEEP_MS (config.h).
  was_busy = pending || kb_count;
  loop_wait(was_busy ? LOOP_BUSY_MS : link_uart_on() ? LOOP_UART_MS : LOOP_SLEEP_MS);
}
