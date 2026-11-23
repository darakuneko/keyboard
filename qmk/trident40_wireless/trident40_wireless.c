// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Trident40 Wireless: STM32L432KB + HY0020 (nRF52832) for BLE, CR2450 coin cell.
// USB when a host is on the cable, else BLE (always automatic).

#include "trident40_wireless.h"
#include "connection.h"
#include "usb_util.h"
#include "link_proto.h"
#include "ble_link.h"
#include "battery.h"
#include "power.h"
#include "hy0020_update.h"

// The host gets the resting value (battery.h); BAT_LVL types it and the value under load right now.
// The last percentage is also kept in the EEPROM (the keyboard word: magic in the upper bytes):
// after power-on there is no resting sample for a minute or so, and the nRF would show 100 %.
#define BATTERY_SAVED_MAGIC 0xBA770000UL
#define BATTERY_SAVE_STEP 3  // percent: saved when it moved this far (flash wear)

static uint8_t battery_saved(void) {
    uint32_t v = eeconfig_read_kb();
    return (v & 0xFFFFFF00UL) == BATTERY_SAVED_MAGIC && (uint8_t)v <= 100 ? (uint8_t)v : 0xFF;
}

static void battery_publish(void) {
    static uint8_t sent = 0xFF;
    uint16_t       mv   = battery_rest_mv();
    if (!mv) return;
    uint8_t percent = battery_percent(mv);
    if (percent == sent) return;
    sent = percent;
    ble_link_battery(percent, mv);
    uint8_t saved = battery_saved();
    if (saved == 0xFF || abs((int)saved - (int)percent) >= BATTERY_SAVE_STEP) eeconfig_update_kb(BATTERY_SAVED_MAGIC | percent);
}

void keyboard_post_init_kb(void) {
    power_init();
    // The output is always automatic (the OUT_* and BT_n keys are gone): undo a saved choice.
    if (connection_get_host_raw() != CONNECTION_HOST_AUTO) connection_set_host(CONNECTION_HOST_AUTO);
    if (power_vbus()) {
        battery_rest_push(battery_read_mv());
    } else if (battery_saved() != 0xFF) {
        ble_link_battery(battery_saved(), 0);  // on battery: the last known level until the first resting sample
    }
    battery_publish();
    keyboard_post_init_user();
}

void housekeeping_task_kb(void) {
    hy0020_update_task();
    // On USB the cell carries no load: any reading is a resting one.
    static uint32_t usb_sample;
    if (power_vbus() && timer_elapsed32(usb_sample) >= BATTERY_USB_SAMPLE_MS) {
        usb_sample = timer_read32();
        battery_rest_push(battery_read_mv());
    }
    battery_publish();
    housekeeping_task_user();
    power_task();
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        power_busy();
        if (!usb_connected_state()) ble_link_wake_nrf();
    }
    if (!process_record_user(keycode, record)) return false;
    if (!record->event.pressed) return true;

    switch (keycode) {
        case QK_BOOT:
            // The ROM DFU is reached over USB only. Entered on battery it just sits there (nothing
            // resets the STM32 until the cell comes out), so without a cable the key does nothing.
            return power_vbus();
        case BAT_LVL: {
            uint16_t rest = battery_rest_mv(), now = battery_read_mv();
            char     buf[32];
            if (rest) {
                snprintf(buf, sizeof(buf), "%u%% %umV now %umV", battery_percent(rest), rest, now);
            } else {
                snprintf(buf, sizeof(buf), "--%% now %umV", now);  // no resting sample yet
            }
            send_string(buf);
            return false;
        }
        case BT_UPDT:
            if (hy0020_update_active()) {
                hy0020_update_stop();
            } else {
                hy0020_update_start();
            }
            return false;
    }
    return true;
}
