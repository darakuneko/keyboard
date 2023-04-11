/* Copyright 2021 daraku-neko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "num_num_black_strawberry.h"
#include "usb_host_os_identifier.h"

#include "i2c_master.h"
#include "pointing_device.h"

#include "iqs5xx.h"
#include "host.h"

static void dummy_func(uint8_t btn){};
void (*pointing_device_set_button)(uint8_t btn) = dummy_func;
void (*pointing_device_clear_button)(uint8_t btn) = dummy_func;

bool mouse_send_flag = false;
bool is_osx = false;

void identify_os(void) { 
    is_osx = get_usb_host_os_type() == 2;
}

void pointing_device_task(void) {
    if (mouse_send_flag) {
        pointing_device_send();
        mouse_send_flag = false;
    }
}

void keyboard_pre_init_kb() {
    i2c_init();
}

void keyboard_post_init_kb() {
    init_iqs5xx();
    pointing_device_set_button = pointing_device_set_button_iqs5xx;
    pointing_device_clear_button = pointing_device_clear_button_iqs5xx;
    ges_time = timer_read32();
    one_finger_drag = false;
        
    wait_ms(300);

    identify_os();
}

void matrix_scan_kb() {
    iqs5xx_data_t iqs5xx_data;
    report_mouse_t mouse_rep = {0};
    bool is_valid = false;

    is_valid = read_iqs5xx(&iqs5xx_data);

    if (is_valid) {
        static iqs5xx_processed_data_t iqs5xx_processed_data;
        static iqs5xx_gesture_data_t iqs5xx_gesture_data;
        bool send_flag = process_iqs5xx(&iqs5xx_data, &iqs5xx_processed_data, &mouse_rep, &iqs5xx_gesture_data);

        switch (iqs5xx_gesture_data.two.gesture_state) {
            case GESTURE_SWIPE_U:
                mouse_rep.v = -2;
                send_flag = true;
                break;
            case GESTURE_SWIPE_D:
                mouse_rep.v = 2;
                send_flag = true;
                break;
            case GESTURE_SWIPE_R:
                if(timer_elapsed32(ges_time) > GES_TIME_MS){
                    is_osx ? tap_code16(LGUI(KC_RBRC)) : tap_code16(KC_WFWD);
                    ges_time = timer_read32();
                }
                break;
            case GESTURE_SWIPE_L:
                if(timer_elapsed32(ges_time) > GES_TIME_MS){
                    is_osx ? tap_code16(LGUI(KC_LBRC)) : tap_code16(KC_WBAK);
                    ges_time = timer_read32();
                }
                break;
            case GESTURE_PINCH_IN:
                if(timer_elapsed32(ges_time) > GES_TIME_MS){
                    is_osx ? tap_code16(LGUI(KC_EQL)) : tap_code16(LCTL(KC_PPLS));
                    ges_time = timer_read32();
                }
                break;
            case GESTURE_PINCH_OUT:
                if(timer_elapsed32(ges_time) > GES_TIME_MS){
                    is_osx ? tap_code16(LGUI(KC_MINS)) : tap_code16(LCTL(KC_PMNS));
                    ges_time = timer_read32();
                }
                break;
            default:
                break;
        }
        if (send_flag) {
            mouse_send_flag = true;
            pointing_device_set_report(mouse_rep);
        }
    }
}
