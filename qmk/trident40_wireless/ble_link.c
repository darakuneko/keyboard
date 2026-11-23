// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// QMK custom Bluetooth driver: reports go to the HY0020 (nRF52832) as frames on USART1 (see
// link_proto.h); the nRF is the BLE HID device. Vial over BLE: the nRF passes the host's raw HID
// reports here, the answer goes back the same way.

#include "quantum.h"
#include <hal.h>
#include "bluetooth.h"
#include "connection.h"
#include "raw_hid.h"
#include "host.h"
#include "usb_util.h"
#include "ble_link.h"
#include "battery.h"
#include "link_proto.h"

static SerialConfig link_config = {LINK_BAUD, 0, 0, 0};
static bool         running;

static uint8_t  bt_state = BT_STATE_IDLE;
static uint8_t  bt_leds;
static uint8_t  sent_percent = 0xFF;  // 0xFF: not measured yet
static uint16_t sent_mv;
static uint32_t last_tx;
static bool     wake_needed = true;      // the nRF may have its receiver off (or be in System OFF)
static uint32_t last_kick;
static ble_link_diag_t diag;
static uint8_t         nrf_stats[30], nrf_stats_len;  // the nRF's last MSG_STATS
static bool     kicked;
static uint8_t  nrf_fault[30], nrf_faults;                    // the nRF's last MSG_FAULT, how many so far

static uint8_t crc8_update(uint8_t crc, uint8_t b) {
    crc ^= b;
    for (uint8_t i = 0; i < 8; i++) crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x07) : (uint8_t)(crc << 1);
    return crc;
}

static void link_send(uint8_t type, const void *payload, uint8_t len) {
    if (!running || len > LINK_MAX_PAYLOAD) return;
    if (wake_needed || timer_elapsed32(last_tx) > LINK_WAKE_AFTER_MS) {
        sdPut(&BT_UART_DRIVER, 0x00);  // falling edge on the nRF's RX: starts its receiver
        wait_ms(LINK_WAKE_MS);
        wake_needed = false;
    }
    uint8_t buf[LINK_MAX_PAYLOAD + 4];
    uint8_t n = 0, crc = 0;
    buf[n++] = LINK_SOF;
    buf[n++] = type;
    buf[n++] = len;
    if (len) memcpy(buf + n, payload, len), n += len;
    for (uint8_t i = 1; i < n; i++) crc = crc8_update(crc, buf[i]);
    buf[n++] = crc;
    sdWrite(&BT_UART_DRIVER, buf, n);
    last_tx = timer_read32();
    diag.tx_frames++;
}

static void link_start(void) {
    palSetLineMode(BT_TX_PIN, PAL_MODE_ALTERNATE(BT_UART_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    palSetLineMode(BT_RX_PIN, PAL_MODE_ALTERNATE(BT_UART_PAL_MODE) | PAL_STM32_PUPDR_PULLUP);
    sdStart(&BT_UART_DRIVER, &link_config);
    running     = true;
    wake_needed = true;
}

// ---- receive -----------------------------------------------------------------------------
enum { RX_SOF, RX_TYPE, RX_LEN, RX_DATA, RX_CRC };
static uint8_t  rx_state = RX_SOF, rx_pos, rx_crc, rx_type, rx_len;
static uint8_t  rx_data[LINK_MAX_PAYLOAD];
static uint32_t rx_last;

// Vial answers through raw_hid_send(), which goes to the active host. The answer belongs to the
// side the request came from: BLE requests make BLE the host while they run, and anything else
// sent here while USB is up (a USB request with the output set to BLE) goes to USB.
static bool in_ble_request;

static void vial_from_host(uint8_t *data) {
#ifdef RAW_ENABLE
    connection_host_t host = connection_get_host_raw();
    connection_set_host_noeeprom(CONNECTION_HOST_BLUETOOTH);
    in_ble_request = true;
    raw_hid_receive(data, 32);
    in_ble_request = false;
    connection_set_host_noeeprom(host);
#endif
}

static void handle_frame(void) {
    diag.rx_frames++;
    switch (rx_type) {
        case MSG_HELLO:
            diag.hellos++;
            memcpy(diag.hello, rx_data, rx_len < sizeof(diag.hello) ? rx_len : sizeof(diag.hello));
            // The nRF (re)started: its receiver is on now; give it what it does not know.
            wake_needed = false;
            last_tx     = timer_read32();
            if (sent_percent != 0xFF) ble_link_battery(sent_percent, sent_mv);
            break;
        case MSG_STATUS:
            if (rx_len >= 1) {
                if (bt_state == BT_STATE_CONNECTED && rx_data[0] != BT_STATE_CONNECTED) clear_keyboard();
                bt_state = rx_data[0];
            }
            if (rx_len >= 5) {
                diag.nrf_rx_frames  = rx_data[1] | (rx_data[2] << 8);
                diag.nrf_rx_dropped = rx_data[3] | (rx_data[4] << 8);
            }
            if (rx_len >= 11) {
                diag.conn_interval = rx_data[5] | (rx_data[6] << 8);
                diag.conn_latency  = rx_data[7] | (rx_data[8] << 8);
                diag.conn_timeout  = rx_data[9] | (rx_data[10] << 8);
            }
            break;
        case MSG_STATS:
            nrf_stats_len = rx_len < sizeof(nrf_stats) ? rx_len : sizeof(nrf_stats);
            memcpy(nrf_stats, rx_data, nrf_stats_len);
            break;
        case MSG_FAULT:
            if (rx_len == sizeof(nrf_fault)) {
                memcpy(nrf_fault, rx_data, sizeof(nrf_fault));
                if (nrf_faults < 255) nrf_faults++;
            }
            break;
        case MSG_BAT_SAMPLE:
            battery_rest_commit();  // published by housekeeping_task_kb()
            break;
        case MSG_LEDS:
            if (rx_len >= 1) bt_leds = rx_data[0];
            break;
        case MSG_VIAL_OUT:
            if (rx_len == 32) vial_from_host(rx_data);
            break;
    }
}

static void link_receive(void) {
    // A gap inside a frame ends it, but only a real gap: the main loop can be late with bytes waiting.
    if (rx_state != RX_SOF && sdGetWouldBlock(&BT_UART_DRIVER) && timer_elapsed32(rx_last) > LINK_BYTE_TIMEOUT_MS) rx_state = RX_SOF;
    msg_t c;
    while ((c = sdGetTimeout(&BT_UART_DRIVER, TIME_IMMEDIATE)) >= 0) {
        rx_last = timer_read32();
        switch (rx_state) {
            case RX_SOF:
                if (c == LINK_SOF) rx_state = RX_TYPE, rx_crc = 0;
                break;
            case RX_TYPE:
                rx_type  = c;
                rx_crc   = crc8_update(rx_crc, c);
                rx_state = RX_LEN;
                break;
            case RX_LEN:
                if (c > LINK_MAX_PAYLOAD) {
                    rx_state = RX_SOF;
                    break;
                }
                rx_len   = c;
                rx_crc   = crc8_update(rx_crc, c);
                rx_pos   = 0;
                rx_state = c ? RX_DATA : RX_CRC;
                break;
            case RX_DATA:
                rx_data[rx_pos++] = c;
                rx_crc            = crc8_update(rx_crc, c);
                if (rx_pos == rx_len) rx_state = RX_CRC;
                break;
            case RX_CRC:
                rx_state = RX_SOF;
                if (c == rx_crc) {
                    handle_frame();
                } else {
                    diag.rx_crc_errors++;
                }
                break;
        }
    }
}

// ---- QMK Bluetooth driver ----------------------------------------------------------------
void bluetooth_init(void) {
    gpio_set_pin_input_high(BT_IRQ_PIN);
    gpio_set_pin_input(BT_NRST_PIN);  // the nRF pulls its reset pin up itself
    gpio_set_pin_input_high(BT_DFU_PIN);
    link_start();
    link_send(MSG_PING, NULL, 0);
}

void bluetooth_task(void) {
    if (running) link_receive();
}

bool bluetooth_is_connected(void) {
    return bt_state == BT_STATE_CONNECTED;
}

uint8_t bluetooth_keyboard_leds(void) {
    return bt_leds;
}

// KEYBOARD, CONSUMER and SYSTEM go twice; the nRF drops the repeat (link_proto.h).
static void link_send_twice(uint8_t type, const void *payload, uint8_t len) {
    link_send(type, payload, len);
    link_send(type, payload, len);
}

void bluetooth_send_keyboard(report_keyboard_t *report) {
    uint8_t r[8] = {report->mods, 0};
    memcpy(r + 2, report->keys, 6);
    link_send_twice(MSG_KEYBOARD, r, sizeof(r));
}

static int8_t clamp8(int16_t v) {
    return v < -127 ? -127 : v > 127 ? 127 : (int8_t)v;
}

void bluetooth_send_mouse(report_mouse_t *report) {
    uint8_t r[5] = {report->buttons, (uint8_t)clamp8(report->x), (uint8_t)clamp8(report->y), (uint8_t)clamp8(report->v), (uint8_t)clamp8(report->h)};
    link_send(MSG_MOUSE, r, sizeof(r));
}

void bluetooth_send_consumer(uint16_t usage) {
    link_send_twice(MSG_CONSUMER, &usage, 2);
}

void bluetooth_send_system(uint16_t usage) {
    link_send_twice(MSG_SYSTEM, &usage, 2);
}

void bluetooth_send_raw_hid(uint8_t *data, uint8_t length) {
    host_driver_t *usb = host_get_driver();
    if (!in_ble_request && usb_connected_state() && usb && usb->send_raw_hid) {
        usb->send_raw_hid(data, length);
        return;
    }
    uint8_t r[32] = {0};
    memcpy(r, data, length > 32 ? 32 : length);
    link_send(MSG_VIAL_IN, r, 32);
}

// ---- keyboard side -----------------------------------------------------------------------
uint8_t ble_link_state(void) {
    return bt_state;
}

void ble_link_command(uint8_t cmd) {
    link_send(MSG_BT_CMD, &cmd, 1);
}

void ble_link_ping(void) {
    link_send(MSG_PING, NULL, 0);
}

void ble_link_request_stats(void) {
    link_send(MSG_STATS_REQ, NULL, 0);
}

uint8_t ble_link_nrf_stats(uint8_t *out) {
    memcpy(out, nrf_stats, nrf_stats_len);
    return nrf_stats_len;
}

uint8_t ble_link_nrf_fault(uint8_t *out) {
    memcpy(out, nrf_fault, sizeof(nrf_fault));
    return nrf_faults;
}

void ble_link_battery(uint8_t percent, uint16_t mv) {
    sent_percent = percent;
    sent_mv      = mv;
    uint8_t r[3]    = {percent, (uint8_t)mv, (uint8_t)(mv >> 8)};
    link_send(MSG_BATTERY, r, sizeof(r));
}

// Keys pressed while BLE is down: the nRF may have given up advertising or be in System OFF.
// Once a second at most (the wake byte alone restarts an nRF in System OFF).
void ble_link_wake_nrf(void) {
    if (bt_state == BT_STATE_CONNECTED) return;
    if (kicked && timer_elapsed32(last_kick) < 1000) return;
    kicked    = true;
    last_kick = timer_read32();
    ble_link_command(BT_CMD_ADVERTISE);
}

const ble_link_diag_t *ble_link_diag(void) {
    diag.state   = bt_state;
    diag.running = running;
    return &diag;
}

bool ble_link_idle(void) {
    if (!running) return true;
    if (rx_state != RX_SOF) return false;
    // No lock: power.c also calls this with the system locked (one unlock here would end its lock).
    if (!iqIsEmptyI(&BT_UART_DRIVER.iqueue)) return false;  // bytes waiting
    // The frame is out when the driver has dropped both interrupts. Not USART_ISR_TC: its interrupt
    // handler clears ISR flags as it reads them, so TC never stays set (seen 2026-11-03: no Stop at all).
    return oqIsEmptyI(&BT_UART_DRIVER.oqueue) && !(USART1->CR1 & (USART_CR1_TXEIE | USART_CR1_TCIE));
}

void ble_link_slept(void) {
    wake_needed = true;
}

void ble_link_release(void) {
    if (!running) return;
    running = false;
    sdStop(&BT_UART_DRIVER);
    rx_state = RX_SOF;
    bt_state = BT_STATE_IDLE;
}

void ble_link_resume(void) {
    if (running) return;
    link_start();
    link_send(MSG_PING, NULL, 0);  // the nRF may have restarted meanwhile (its HELLO went nowhere)
}
