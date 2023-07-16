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
#include "quantum.h"
#include "drivers/haptic/DRV2605L.h"

uint16_t iqs_regaddr = IQS5xx_READ_ADDR<<1;
static inline uint8_t iqs_app_writeReg_continue(uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_writeReg16(iqs_regaddr, regaddr, data, len, 100);
}

static inline uint8_t iqs_app_readReg_continue(uint16_t regaddr, uint8_t* data, uint16_t len) {
    i2c_status_t status = i2c_readReg16(iqs_regaddr, regaddr, data, len, 100);
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

void init_iqs5xx(void) {
    uint8_t data = 0x80; //LP Mode
    iqs_app_writeReg(IQS5xx_SYSTEM_CTRL0, &data, 1);
    uint16_t default_addr = IQS5xx_FINGER_NUM << 8; //range
    iqs_app_writeReg(IQS5xx_DEFAULT_READ, (uint8_t*)&default_addr, sizeof(default_addr));
    uint8_t distance = 0x02; //Zoom initial distance
    iqs_app_writeReg(IQS5xx_ZOOM, &distance, 1);    
}

bool read_iqs5xx(iqs5xx_data_t* const data) {
    uint8_t i2c_res = iqs_app_readReg_continue(IQS5xx_FINGER_NUM, &data->finger_cnt, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT0, &data->ges_evnet0, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT1, &data->ges_evnet1, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_RELATIVE_XY, (uint8_t*)&data->relative_xy, 4);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_TOUCH_STRENGTH_FINGER_THREE, &data->touch_strenght_finger_three, 1);

    bool res = false;
    if (i2c_res != 0) {
        res = false;
    } else {
        iqs_app_end_communication();
        res = true;
    }

    return res;
}

bool tapped = false; 
void set_tap(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {    
    if(tapped && (data->finger_cnt == 0 || !use_drag)){
        rep_mouse->buttons &= ~(1 << (KC_BTN1 - KC_BTN1));
        clear_buttons = true;
        tapped = false;
        return;
    } 

    if (data->ges_evnet0 == 1) {    
        if(use_drag){
            use_drag = false;
        } else {
            rep_mouse->buttons |= (1 << (KC_BTN1 - KC_BTN1));
            tapped = true;
        }
    } else if (timer_elapsed32(tap_time) > TAP_TERM && !use_drag && data->ges_evnet1 == 1) {
        rep_mouse->buttons |= (1 << (KC_BTN2 - KC_BTN1));
        tapped = true;
    } else if(can_drag && !use_drag && data->ges_evnet0 == 2) {
        if(drag_time == 0) {
            drag_time = timer_read32();
        } else if(timer_elapsed32(drag_time) > drag_term){
            DRV_pulse(hf_waveform_number); 
            use_drag = true;
            drag_time = 0;
            tapped = true;
        }
    } else if(timer_elapsed32(tap_time) > TAP_TERM && data->touch_strenght_finger_three < 255 && data->ges_evnet1 == 0) {
        data->gesture = TAP_FINGER_THREE;
    }

    if(rep_mouse->buttons || data->gesture == TAP_FINGER_THREE){
        tap_time = timer_read32();
    }
    
    if (use_drag){
        rep_mouse->buttons |= (1 << (KC_BTN1 - KC_BTN1));
    }

    if(data->finger_cnt == 0){
        drag_time = 0;
    } 
}

void set_gesture(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {
    uint32_t dx = ((data->relative_xy.bytes[1] - data->relative_xy.bytes[0]) / 2) * accel_speed;
    uint32_t dy = ((data->relative_xy.bytes[3] - data->relative_xy.bytes[2]) / 2) * accel_speed;
    if (data->finger_cnt == 1) {
        rep_mouse->x = dx;
        rep_mouse->y = dy;
    } else if (data->finger_cnt >= 2) {
        if(timer_elapsed32(swipe_time) > SWIPE_TERM && data->ges_evnet1 == 2 && data->finger_cnt == 2) {
            if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_L;
            } else if(data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_R;
            } 

            swipe_time = timer_read32();
        } else if(timer_elapsed32(gesture_time) > GESTURE_TERM && data->ges_evnet1 == 2 && data->finger_cnt == 3) {
            if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_L;
            } else if(data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_R;
            } else if(data->relative_xy.bytes[2] > 1 && data->relative_xy.bytes[3] > 1 ){
                data->gesture = GESTURE_SWIPE_D;
            } else if(data->relative_xy.bytes[3] > 1 ){
                data->gesture = GESTURE_SWIPE_U;
            }

            gesture_time = timer_read32();
        } else if(timer_elapsed32(scroll_time) > scroll_term && data->ges_evnet1 == 2) {
            if(data->relative_xy.bytes[2] > 1 && data->relative_xy.bytes[3] > 1 ){
                rep_mouse->v = 1;
            } else if(data->relative_xy.bytes[3] > 1 ){
                rep_mouse->v = -1;
            } 
            scroll_time = timer_read32();
        } else if(timer_elapsed32(pinch_time) > PINCH_TERM && data->ges_evnet1 == 4) {
            if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0) {
                data->gesture = GESTURE_PINCH_OUT;
            } else if(data->relative_xy.bytes[1] > 0) {
                data->gesture = GESTURE_PINCH_IN;
            }
            pinch_time = timer_read32();
        }
    }
}

void set_trackpad_layer(iqs5xx_data_t* const data) {
    if (data->finger_cnt == 0) {
        if(!is_press_ms_btn && use_trackpad_layer && can_trackpad_layer){
            layer_move(get_highest_layer(default_layer_state));
            use_trackpad_layer = false;
        }
   } else {
        if(!use_trackpad_layer && can_trackpad_layer){
            layer_move(trackpad_layer);
            use_trackpad_layer = true;
        }
    }
}

void process_iqs5xx(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {
    set_trackpad_layer(data);
    if(can_tap) {
        set_tap(data, rep_mouse);
    }
    if(!rep_mouse->buttons || data->gesture != TAP_FINGER_THREE){
        set_gesture(data, rep_mouse);
    }
}

