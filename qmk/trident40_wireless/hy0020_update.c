// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// HY0020 update over USB: the STM32 tunnels the nRF bootloader's serial DFU (115200 baud, the
// Adafruit nRF52 bootloader) through Vial's raw HID interface, so a browser (WebHID) or a script
// can write the HY0020 without a USB-serial adapter.
//
// Update mode starts only from the keyboard (the BT_UPDT key, USB connected): the host cannot
// switch it on by itself. It ends on BT_UPDT again, on an "exit" request or when USB goes away; the
// nRF is then reset into its application and the BLE link comes back.
//
// Entering the bootloader: update mode asks the HY0020 application for it (BT_CMD_ENTER_DFU, the
// bootloader then waits in serial DFU). For an application that does not answer that, the host can
// ask for a reset pulse (UPD_CTRL op 1): after a pin reset the bootloader listens for serial DFU
// for a short while (what a USB-serial adapter's DTR auto-reset uses; adafruit-nrfutil sends its
// first packet 0.1 s after the reset). That needs P0.21 set up as the reset pin (UICR PSELRESET),
// which bare HY0020 modules did not have (seen 2026-10-29; the HY0020 firmware sets it).
// P0.20, the documented DFU pin, is not used.
//
// Raw HID commands (32-byte reports, byte 0 = command):
//   UPD_WRITE  host -> kbd: [0xB0, n, data[n]] (n <= 30) bytes for the nRF. Answer: the same report
//              with byte 1 = number of bytes taken (0 when not in update mode).
//   UPD_READ   kbd -> host, unsolicited: [0xB1, n, data[n]] bytes from the nRF.
//   UPD_CTRL   host -> kbd: [0xB2, op]. op 0 = state, 1 = reset into the bootloader (send the first
//              DFU packet within ~0.1 s of the answer), 2 = exit, 3 = UART baud rate = u32 at
//              byte 2 (to reach an application on another link speed; 115200 is the bootloader's),
//              4 = leave update mode and restart the STM32 in its ROM bootloader (USB DFU), so a
//              browser can write the STM32 too without QK_BOOT / Vial unlock. Like every op but 0
//              it only works in update mode, which starts from a key press.
//              Answer: [0xB2, op, active (0/1)].
//   DIAG       host -> kbd: [0xB3]. Answer: [0xB3, ble_link_diag_t (26 bytes), u16 battery mV,
//              u8 vbus, u8 usb state]. Works in and out of update mode.
//   POWER      host -> kbd: [0xB4]. Answer: [0xB4, power_diag_t (29 bytes)].
//   ERASE      host -> kbd: [0xB5]. Erase every BLE pairing (BT_CMD_ERASE_ALL). Answer: [0xB5, sent]
//              (0 in update mode or without USB).
//   NRF_STATS  host -> kbd: [0xB6]. Answer: [0xB6, n, the nRF's last MSG_STATS payload (n bytes)]
//              and asks the nRF for fresh ones (ask again ~0.3 s later for those).
//   NRF_FAULT  host -> kbd: [0xB8, op]. Answer: [0xB8, count since the STM32 started, the nRF's
//              last MSG_FAULT payload (30 bytes)]: its loop() stalled (link_proto.h). op 0xA5 also
//              makes the nRF stall on purpose (BT_CMD_TEST_STALL: the watchdog restarts it), to
//              check the record end to end.
//   BATTERY    host -> kbd: [0xB7]. Answer: [0xB7, n, u16 resting battery mV (median), u16 x n
//              resting samples, newest last] (battery.h).

#include "quantum.h"
#include <hal.h>
#include "connection.h"
#include "usb_util.h"
#include "raw_hid.h"
#include "ble_link.h"
#include "power.h"
#include "battery.h"
#include "usb_main.h"
#include "hy0020_update.h"
#include "link_proto.h"

#define UPD_WRITE 0xB0
#define UPD_READ 0xB1
#define UPD_CTRL 0xB2
#define UPD_DIAG 0xB3
#define UPD_POWER 0xB4
#define UPD_ERASE 0xB5
#define UPD_NRF_STATS 0xB6
#define UPD_BATTERY 0xB7
#define UPD_NRF_FAULT 0xB8

#define UPD_BAUD 115200

static SerialConfig      dfu_config = {UPD_BAUD, 0, 0, 0};
static bool              active;
static connection_host_t saved_host;
static uint32_t          boot_at;  // op 4: jump to the ROM bootloader at this time (0 = no)

// Returns right after the reset pulse: the bootloader's listening window starts now.
static void nrf_reset(void) {
    gpio_set_pin_output(BT_NRST_PIN);
    gpio_write_pin_low(BT_NRST_PIN);
    wait_ms(2);
    gpio_set_pin_input(BT_NRST_PIN);
}

bool hy0020_update_active(void) {
    return active;
}

void hy0020_update_start(void) {
    if (active || !usb_connected_state()) return;
    active     = true;
    saved_host = connection_get_host_raw();
    connection_set_host_noeeprom(CONNECTION_HOST_USB);  // the tunnel's answers go to USB
    clear_keyboard();
    ble_link_command(BT_CMD_ENTER_DFU);
    wait_ms(5);  // the frame is out (40 us at 1 Mbaud, after the wake byte's LINK_WAKE_MS)
    ble_link_release();
    palSetLineMode(BT_TX_PIN, PAL_MODE_ALTERNATE(BT_UART_PAL_MODE) | PAL_OUTPUT_TYPE_PUSHPULL | PAL_OUTPUT_SPEED_HIGHEST);
    palSetLineMode(BT_RX_PIN, PAL_MODE_ALTERNATE(BT_UART_PAL_MODE) | PAL_STM32_PUPDR_PULLUP);
    dfu_config.speed = UPD_BAUD;
    sdStart(&BT_UART_DRIVER, &dfu_config);
    // no reset pulse here: it would restart the nRF while it is entering the bootloader
    while (sdGetTimeout(&BT_UART_DRIVER, TIME_IMMEDIATE) >= 0) {
    }
}

void hy0020_update_stop(void) {
    if (!active) return;
    active = false;
    sdStop(&BT_UART_DRIVER);
    nrf_reset();
    ble_link_resume();
    connection_set_host_noeeprom(saved_host);
}

void hy0020_update_task(void) {
    if (boot_at && timer_expired32(timer_read32(), boot_at)) {
        boot_at = 0;
        hy0020_update_stop();  // the nRF goes back to its application
        if (usb_connected_state()) reset_keyboard();  // does not return
    }
    if (!active) return;
    power_busy();
    if (!usb_connected_state()) {
        hy0020_update_stop();
        return;
    }
    uint8_t r[32] = {UPD_READ};
    uint8_t n     = 0;
    msg_t   c;
    while (n < 30 && (c = sdGetTimeout(&BT_UART_DRIVER, TIME_IMMEDIATE)) >= 0) {
        r[2 + n++] = (uint8_t)c;
    }
    if (n) {
        r[1] = n;
        raw_hid_send(r, sizeof(r));
    }
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        case UPD_WRITE: {
            uint8_t n = data[1] > 30 ? 30 : data[1];
            if (active) {
                sdWrite(&BT_UART_DRIVER, &data[2], n);
                data[1] = n;
            } else {
                data[1] = 0;
            }
            break;
        }
        case UPD_CTRL:
            if (active && data[1] == 1) nrf_reset();
            if (active && data[1] == 2) hy0020_update_stop();
            if (active && data[1] == 3) {
                uint32_t baud = data[2] | (data[3] << 8) | ((uint32_t)data[4] << 16) | ((uint32_t)data[5] << 24);
                if (baud >= 9600 && baud <= 1000000) {
                    sdStop(&BT_UART_DRIVER);
                    dfu_config.speed = baud;
                    sdStart(&BT_UART_DRIVER, &dfu_config);
                }
            }
            if (active && data[1] == 4) boot_at = (timer_read32() + 100) | 1;  // after the answer is out
            data[2] = active;
            break;
        case UPD_DIAG: {
            if (!active) ble_link_ping();  // fresh nRF counters for the next DIAG
            const ble_link_diag_t *d = ble_link_diag();
            uint8_t                n = sizeof(*d);
            _Static_assert(sizeof(*d) + 5 <= 32, "DIAG answer is one raw HID report");
            memcpy(&data[1], d, n);
            uint16_t mv = battery_read_mv();
            data[1 + n] = (uint8_t)mv;
            data[2 + n] = (uint8_t)(mv >> 8);
            data[3 + n] = power_vbus();
            data[4 + n] = USB_DRIVER.state;
            break;
        }
        case UPD_ERASE:
            // Not in update mode: the HY0020 application must be running to take the command.
            // Only with the USB cable in: the format erases flash pages, too much for the coin cell.
            data[1] = !active && usb_connected_state();
            if (data[1]) ble_link_command(BT_CMD_ERASE_ALL);
            break;
        case UPD_NRF_STATS:
            if (!active) ble_link_request_stats();
            data[1] = ble_link_nrf_stats(&data[2]);
            break;
        case UPD_NRF_FAULT:
            if (!active && data[1] == 0xA5) ble_link_command(BT_CMD_TEST_STALL);
            data[1] = ble_link_nrf_fault(&data[2]);
            break;
        case UPD_BATTERY: {
            // [0xB7, n, u16 resting median, u16 samples[n]] (newest last)
            uint16_t v[BATTERY_REST_SAMPLES + 1];
            v[0]    = battery_rest_mv();
            data[1] = battery_rest_samples(&v[1]);
            memcpy(&data[2], v, sizeof(v));
            break;
        }
        case UPD_POWER: {
            _Static_assert(sizeof(power_diag_t) + 1 <= 32, "POWER answer is one raw HID report");
            memcpy(&data[1], power_diag(), sizeof(power_diag_t));
            break;
        }
        default:
            data[0] = 0xFF;  // id_unhandled
            break;
    }
}
