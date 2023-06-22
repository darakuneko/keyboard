/* Copyright 2020 sekigon-gonnoc
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

#include "iqs5xx.h"
#include "i2c_master.h"
#include "quantum.h"
#include "debug.h"
#include <string.h>  // memset

#define IQS5xx_COM_END_REG 0xEEEE
#define IQS5xx_BL_STATUS 0x0006
#define IQS5xx_SYSTEM_INFO0 0x000F
#define IQS5xx_FINGER_NUM 0x0011
#define IQS5xx_F1_POSX 0x0016
#define IQS5xx_F2_POSX 0x001D
#define IQS5xx_COUNT_VALS 0x0095
#define IQS5xx_REF_VALS 0x0303
#define IQS5xx_SYSTEM_CTRL0 0x0431
#define IQS5xx_SYSTEM_CTRL1 0x0432
#define IQS5xx_FINGER_SPLIT 0x066B
#define IQS5xx_DEFAULT_READ 0x0675


#define iqs_app_writeReg_continue(regaddr, data, len) iqs_writeReg_continue(IQS5xx_READ_ADDR<<1, regaddr, data, len)
#define iqs_app_readReg_continue(regaddr, data, len) iqs_readReg_continue(IQS5xx_READ_ADDR<<1, regaddr, data, len)

static uint8_t pointing_device_button;
static bool    send_flag = false;  // new mouse motion is detected and it should be send to host

void pointing_device_set_button_iqs5xx(uint8_t btn) {
    pointing_device_button |= btn;
    send_flag = true;
}

void pointing_device_clear_button_iqs5xx(uint8_t btn) {
    pointing_device_button &= ~btn;
    send_flag = true;
}

static inline uint8_t iqs_writeReg_continue(uint8_t devaddr, uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_writeReg16(devaddr, regaddr, data, len, 100);
}

static inline uint8_t iqs_readReg_continue(uint8_t devaddr, uint16_t regaddr, uint8_t* data, uint16_t len) {
    i2c_status_t status = i2c_readReg16(devaddr, regaddr, data, len, 100);
    return (status < 0) ? status : I2C_STATUS_SUCCESS;
}

static inline uint8_t iqs_app_end_communication(void) {
    uint8_t _ = 0xFF;
    return iqs_app_writeReg_continue(IQS5xx_COM_END_REG, &_, 1);
}

static inline uint8_t iqs_app_writeReg(uint16_t regaddr, uint8_t* data, uint16_t len) {
    uint8_t res = iqs_app_writeReg_continue(regaddr, data, len);
    res |= iqs_app_end_communication();

    return res;
}

static inline uint8_t iqs_app_readReg(uint16_t regaddr, uint8_t* data, uint16_t len) {
    uint8_t res = iqs_app_readReg_continue(regaddr, data, len);
    res |= iqs_app_end_communication();

    return res;
}


uint16_t check_iqs5xx() {
    uint8_t dat[2] = {0};

    uint8_t res = iqs_app_readReg(0, dat, sizeof(dat));

    if (res) {
        // retry once to avoid LP mode
        res = iqs_app_readReg(0, dat, sizeof(dat));

        if (res) return 0;
    }

    uint16_t pid = ((uint16_t)dat[0] << 8) | dat[1];

    if (pid == 40 || pid == 58 || pid == 52) {
        return pid;
    } else {
        return 0;
    }
}

int init_iqs5xx(void) {
    uint8_t data = 0x80;
    uint8_t res = iqs_app_writeReg(IQS5xx_SYSTEM_CTRL0, &data, 1);

    if (res) {
        // retry once to avoid LP mode
        res = iqs_app_writeReg(IQS5xx_SYSTEM_CTRL0, &data, 1);
        if (res) return 0;
    }

    uint16_t default_addr = IQS5xx_FINGER_NUM << 8;
    iqs_app_writeReg(IQS5xx_DEFAULT_READ, (uint8_t*)&default_addr, sizeof(default_addr));
    return 0;
}

#define swap(type, a, b)     \
    {                        \
        type c;              \
        c = a, a = b, b = c; \
    }

bool read_iqs5xx(iqs5xx_data_t* const data) {
    bool res = false;

    for (int idx = 0; idx < FINGER_MAX; idx++) {
        data->fingers[idx].current.x = 0xFFFF;
        data->fingers[idx].current.y = 0xFFFF;
    }
    data->finger_cnt = 0;
    data->mode       = 0;

    uint8_t i2c_res = iqs_app_readReg_continue(IQS5xx_FINGER_NUM, &data->finger_cnt, 1);

    if (data->finger_cnt >= 1) {
        i2c_res |= iqs_app_readReg_continue(IQS5xx_F1_POSX, (uint8_t*)&data->fingers[0], 4);
        swap(uint8_t, data->fingers[0].bytes[0], data->fingers[0].bytes[1]);
        swap(uint8_t, data->fingers[0].bytes[2], data->fingers[0].bytes[3]);
    }

    if (data->finger_cnt >= 2) {
        i2c_res |= iqs_app_readReg_continue(IQS5xx_F2_POSX, (uint8_t*)&data->fingers[1], 4);
        swap(uint8_t, data->fingers[1].bytes[0], data->fingers[1].bytes[1]);
        swap(uint8_t, data->fingers[1].bytes[2], data->fingers[1].bytes[3]);
    }

    if (data->finger_cnt >= 3) {
        i2c_res |= iqs_app_readReg_continue(IQS5xx_F2_POSX, (uint8_t*)&data->fingers[2], 4);
        swap(uint8_t, data->fingers[2].bytes[0], data->fingers[2].bytes[1]);
        swap(uint8_t, data->fingers[2].bytes[2], data->fingers[2].bytes[3]);
    }

    if (i2c_res != 0) {
        res = false;
    } else {
        iqs_app_end_communication();
        res = true;
    }

    return res;
}

// return atan2 value in int16_t
#define MY_PI (32767U)
static int16_t myatan2(int16_t y, int16_t x) {
    const int16_t  coeff1 = MY_PI / 4;
    const uint16_t coeff2 = 3 * coeff1;
    int32_t        abs_y;
    int16_t        r, angle;

    if (y < 0) {
        abs_y = -(y << 8);
    } else if (y == 0) {
        abs_y = 1;
    } else {
        abs_y = (y << 8);
    }
    x <<= 8;
    if (x >= 0) {
        r     = (x - abs_y) / ((x + abs_y) >> 8);
        angle = coeff1 - (((int32_t)coeff1 * r) >> 8);
    } else {
        r     = (x + abs_y) / ((abs_y - x) >> 8);
        angle = coeff2 - (((int32_t)coeff1 * r) >> 8);
    }
    if (y < 0)
        return (-angle);  // negate if in quad III or IV
    else
        return (angle);
}

static void recognize_gesture(iqs5xx_data_t const* const data, iqs5xx_processed_data_t* processed, iqs5xx_gesture_data_t* gesture_data) {
    // vector from finger 1 to 0
    int32_t dx                = ((int32_t)processed->fingers[0].last.x - processed->fingers[1].last.x);
    int32_t dy                = ((int32_t)processed->fingers[0].last.y - processed->fingers[1].last.y);
    gesture_data->multi.dot     = processed->fingers[0].dx * processed->fingers[1].dx + processed->fingers[0].dy * processed->fingers[1].dy;
    gesture_data->multi.dot_rel = (((int32_t)processed->fingers[0].rel.x * processed->fingers[1].rel.x)) + (((int32_t)processed->fingers[0].rel.y * processed->fingers[1].rel.y));
    gesture_data->multi.dist_sq = ((dx * dx) >> 5) + ((dy * dy) >> 5);

    // Calc initial distance of fingres
    if (gesture_data->multi.dist_sq_init == 0) {
        int32_t dx0                    = ((int32_t)processed->fingers[0].start.x - processed->fingers[1].start.x);
        int32_t dy0                    = ((int32_t)processed->fingers[0].start.y - processed->fingers[1].start.y);
        gesture_data->multi.dist_sq_init = ((dx0 * dx0) >> 5) + ((dy0 * dy0) >> 5);
    }

    // calc distance change
    int16_t dist_diff = (gesture_data->multi.dist_sq) - (gesture_data->multi.dist_sq_init);

    if (gesture_data->multi.gesture_state == GESTURE_NONE) {
        if (gesture_data->multi.dot_rel > MIN_MOVE_FOR_GES && dist_diff < (800)) {
            gesture_data->multi.gesture_state = GESTURE_SWIPE_IDLE;
            //dprintf("NONE->SWIPE: dot_rel(%d), dist_diff(%d)\n", gesture_data->multi.dot_rel, dist_diff);
        } else if (gesture_data->multi.dot_rel < -MIN_MOVE_FOR_GES) {
            if (dist_diff < (-1500) || dist_diff > (1500)) {
                gesture_data->multi.gesture_state = GESTURE_PINCH_IDLE;
                //dprintf("NONE->PINCH: dot_rel(%d), dist_diff(%d)\n", gesture_data->multi.dot_rel, dist_diff);
            } else {
                gesture_data->multi.gesture_state = GESTURE_ROT_IDLE;
                //dprintf("NONE->ROT: dot_rel(%d), dist_diff(%d)\n", gesture_data->multi.dot_rel, dist_diff);
            }
        }
    }

    switch (gesture_data->multi.gesture_state) {
        case GESTURE_SWIPE_IDLE ... GESTURE_SWIPE_END:
            if (gesture_data->multi.dot > MIN_MOVE_FOR_SWIPE_UPDATE) {
                int16_t angle_s = myatan2(processed->fingers[0].dy, processed->fingers[1].dx);
   
                // convert angle [-pi, pi] to [0, 2pi]
                uint16_t angle = angle_s > 0 ? angle_s : (0xFFFF + angle_s);

                // classify to 8 direction
                gesture_data->multi.gesture_state = GESTURE_SWIPE_R;
                for (uint16_t idx = 0; idx < 7; idx++) {
                    if (angle > MY_PI / 8U * (idx * 2 + 1) && angle <= MY_PI / 8U * (idx * 2 + 3)) {
                        gesture_data->multi.gesture_state = (GESTURE_SWIPE_RU + idx);
                    }
                }
            } else {
                gesture_data->multi.gesture_state = GESTURE_SWIPE_IDLE;
            }
            break;

        case GESTURE_PINCH_IDLE ... GESTURE_PINCH_OUT:
            if (gesture_data->multi.dot < -MIN_MOVE_FOR_PINCH_UPDATE) {
                // vector indicate relative velocity
                int16_t v_x = (processed->fingers[0].dx - processed->fingers[1].dx);
                int16_t v_y = (processed->fingers[0].dy - processed->fingers[1].dy);
                // pinch in/out is distinguished by dot product of vectors
                if (dx * v_x + dy * v_y > 0) {
                    gesture_data->multi.gesture_state = GESTURE_PINCH_IN;
                } else {
                    gesture_data->multi.gesture_state = GESTURE_PINCH_OUT;
                }
            }
            else {
                gesture_data->multi.gesture_state = GESTURE_PINCH_IDLE;
            }
            break;

        case GESTURE_ROT_IDLE ... GESTURE_ROT_CCW: {
            // vector indicate relative velocity
            int16_t v_x = (processed->fingers[0].dx - processed->fingers[1].dx);
            int16_t v_y = (processed->fingers[0].dy - processed->fingers[1].dy);

            // rot direction is distinguished by cross product of vectors
            int16_t rot = (dx * v_y) - (dy * v_x);

            if (rot > MIN_MOVE_FOR_ROT_UPDATE) {
                gesture_data->multi.gesture_state = GESTURE_ROT_CCW;
            } else if (rot < -MIN_MOVE_FOR_ROT_UPDATE) {
                gesture_data->multi.gesture_state = GESTURE_ROT_CW;
            } else {
                gesture_data->multi.gesture_state = GESTURE_ROT_IDLE;
            }
        } break;

        default:
            break;
    }
}

bool process_iqs5xx(iqs5xx_data_t const* const data, iqs5xx_processed_data_t* processed, report_mouse_t* const rep_mouse, iqs5xx_gesture_data_t* gesture_data) {
    uint8_t active_finger_id = 0;      // indicate which finger is active. Used for cursor move
    bool    tapped           = false;  // tapping motion detection flag

    for (int idx = 0; idx < FINGER_MAX; idx++) {
        if (data->fingers[idx].current.x == 0xFFFF || data->fingers[idx].current.y == 0xFFFF) {
            // finger is up
            if (processed->fingers[idx].state == FINGER_DOWN) {
                processed->fingers[idx].state = FINGER_UP;

                // detect tapping motion
                if(idx == 0 || idx == 1 || idx == 2){
                    if (
                        abs(processed->fingers[idx].rel.x) < 20 && 
                        abs(processed->fingers[idx].rel.y) < 20 &&
                        timer_elapsed32(processed->fingers[0].t_tapped) < TAP_TIME_MS
                        ) {
                            processed->fingers[idx].t_tapped = timer_read32();
                            processed->fingers[idx].tapped   = true;
                            tapped                           = true;
                    }
                }
            }

            processed->fingers[idx].dx    = 0;
            processed->fingers[idx].dy    = 0;
            processed->fingers[idx].rel.x = 0;
            processed->fingers[idx].rel.y = 0;
            processed->fingers[idx].frame_move.x = 0;
            processed->fingers[idx].frame_move.y = 0;
        } else {
            // finger is down
            //uprintf("finger: %s\n", "down");
            active_finger_id = idx;
            if (processed->fingers[idx].state == FINGER_UP) {
                processed->fingers[idx].state   = FINGER_DOWN;
                processed->fingers[idx].start   = data->fingers[idx].current;
                processed->fingers[idx].last    = data->fingers[idx].current;
                processed->fingers[idx].t_start = timer_read32();

                processed->fingers[idx].last_gesture_point = data->fingers[idx].current;
            } else {
                processed->fingers[idx].dx   = ((int32_t)data->fingers[idx].current.x - processed->fingers[idx].last_gesture_point.x) * IQS_X_DIR;
                processed->fingers[idx].dy   = ((int32_t)data->fingers[idx].current.y - processed->fingers[idx].last_gesture_point.y) * IQS_Y_DIR;
                processed->fingers[idx].frame_move.x   = ((int32_t)data->fingers[idx].current.x - processed->fingers[idx].last.x) * IQS_X_DIR;
                processed->fingers[idx].frame_move.y   = ((int32_t)data->fingers[idx].current.y - processed->fingers[idx].last.y) * IQS_Y_DIR;
                processed->fingers[idx].last = data->fingers[idx].current;

                processed->fingers[idx].rel.x = (data->fingers[idx].current.x - processed->fingers[idx].start.x) * IQS_X_DIR;
                processed->fingers[idx].rel.y = (data->fingers[idx].current.y - processed->fingers[idx].start.y) * IQS_Y_DIR;
            }
        }
    }

    memset(rep_mouse, 0, sizeof(*rep_mouse));

    // check buttons should be released
    if (processed->tap_cnt > 0) {
        uint8_t button = (1 << (processed->tap_cnt - 1));
        if (!(pointing_device_button & button)) {
            processed->tap_cnt = 0;
            send_flag          = true;
        }
    }

    if (tap_mode && pointing_device_button != 0) {
        rep_mouse->buttons |= pointing_device_button;
    }
    // if any finger taps and all fingers are released process tapping
    if (data->finger_cnt == 0) {
        iqs5xx_gesture_data_t zero = {0};
        *gesture_data              = zero;

        if (tap_mode && tapped) {
            // check tapping
            processed->tap_cnt = 0;
            for (int idx = 0; idx < FINGER_MAX; idx++) {
                if (processed->fingers[idx].tapped) {
                    processed->tap_cnt++;
                }
                processed->fingers[idx].tapped = false;
            }
            if (!hold_drag_mode && processed->tap_cnt < 3) {
                rep_mouse->buttons = (1 << (processed->tap_cnt - 1));
                send_flag = true;
            }

            hold_drag_mode = false;
            hold_drag_time = timer_read32();
            pointing_device_clear_button_iqs5xx(1 << (KC_BTN1 - KC_BTN1));
        }
        processed->fingers[0].t_tapped = timer_read32();
        hold_drag_mode = false;
        hold_drag_time = timer_read32();
    } else if (data->finger_cnt == 1) {
        // Hold click to drag if another finger is tapped
        int8_t diff_x = processed->fingers[active_finger_id].last_gesture_point.x - processed->fingers[active_finger_id].last.x;
        int8_t diff_y = processed->fingers[active_finger_id].last_gesture_point.y - processed->fingers[active_finger_id].last.y;
        if (tap_mode && hold_drag_mode) {
            pointing_device_set_button_iqs5xx(1 << (KC_BTN1 - KC_BTN1));
        } 

        if (diff_x < 2 && diff_y  < 2) {
            if(!hold_drag_mode && timer_elapsed32(hold_drag_time) > DRAG_TIME_MS) {
                hold_drag_mode = true;
            }
        } else {
            if(!hold_drag_mode) {
                hold_drag_time = timer_read32();
            }
        }
        rep_mouse->x                                            = processed->fingers[active_finger_id].frame_move.x / 2;
        rep_mouse->y                                            = processed->fingers[active_finger_id].frame_move.y / 2;
        processed->fingers[active_finger_id].last_gesture_point = processed->fingers[active_finger_id].last;
        send_flag                                               = true;
    } else if ((data->finger_cnt == 2 || data->finger_cnt == 3) && (timer_elapsed32(processed->fingers[1].t_tapped) > TAP_TIME_MS)) {
        recognize_gesture(data, processed, gesture_data);
        if ((gesture_data->multi.gesture_state & 0x0F) != 0x00) {
            processed->fingers[0].last_gesture_point = data->fingers[0].current;
            processed->fingers[1].last_gesture_point = data->fingers[1].current;
            processed->fingers[2].last_gesture_point = data->fingers[2].current;
        }
    } else {
        gesture_data->multi.gesture_state = GESTURE_NONE;
        gesture_data->multi.dist_sq_init  = 0;
    }

    bool ret_val = send_flag;
    send_flag = false;

    return ret_val;
}