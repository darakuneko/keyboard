/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "i2c_master.h"
#include "iqs5xx.h"
#include "quantum.h"
#include "drivers/haptic/drv2605l.h"

uint16_t iqs_device_addr = IQS5xx_DEVICE_ADDR<<1;

static inline uint8_t iqs_app_readReg_continue(uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_readReg16(iqs_device_addr, regaddr, data, len, 100);
}

static inline uint8_t iqs_app_end_communication(void) {
    uint8_t _ = 0xFF;
    return i2c_writeReg16(iqs_device_addr, IQS5xx_COM_END_REG, &_, 1, 100);
}

static inline uint8_t iqs_app_writeReg(uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_writeReg16(iqs_device_addr, regaddr, data, len, 100);
}

void iqs_app_writeRegR(uint16_t regaddr, uint8_t* data, uint16_t len) {
    int retry_count = 10;
    while (retry_count > 0) {
        uint8_t res = iqs_app_writeReg(regaddr, data, len);
        if(res == I2C_STATUS_SUCCESS){
            break;
        }
        wait_ms(100);
        retry_count--;
    }
}

//set absolutely
void init_iqs5xx(void) {
    uint16_t default_addr = IQS5xx_FINGER_NUM << 8;
    iqs_app_writeRegR(IQS5xx_DEFAULT_READ, (uint8_t*)&default_addr, sizeof(default_addr));
    uint8_t distance[] = {0x01, 0xA0};
    iqs_app_writeRegR(IQS5xx_ZOOM, distance, 2);
    uint8_t hold_time = 0x00;
    iqs_app_writeRegR(IQS5xx_HOLD_TIME, &hold_time, 1);
}

bool read_iqs5xx(iqs5xx_data_t* const data) {
    uint8_t i2c_res = iqs_app_readReg_continue(IQS5xx_FINGER_NUM, &data->finger_cnt, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT0, &data->ges_evnet0, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT1, &data->ges_evnet1, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_RELATIVE_XY, (uint8_t*)&data->relative_xy, 4);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_ABSOLUTE_XY, (uint8_t*)&data->absolute_xy, 4);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_TOUCH_STRENGTH_FINGER1, (uint8_t*)&data->touch_strenght1, 1);
    i2c_res |= iqs_app_readReg_continue(IQS5xx_TOUCH_STRENGTH_FINGER3, &data->touch_strenght3, 1);

    bool res = i2c_res == 0;
    if (res) {
        iqs_app_end_communication();
    }
    return res;
}

int tapped3_cnt = 0;
void set_tap(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {    
    if(tapped && data->finger_cnt == 0){
        rep_mouse->buttons = 0;
        clear_buttons = true;
        tapped = false;
        use_drag = false;
        return;  
    }

    if(data->finger_cnt == 0 && tapped3_cnt > FINGER_THREE_TAP_CNT){
        data->gesture = TAP_FINGER_THREE;
    } 
    
    if (data->ges_evnet0 == 1) {  
        if(data->absolute_xy.bytes[0] == 0 && data->absolute_xy.bytes[1] < 128){
            data->gesture = TAP_FINGER_ONE_LEFT;
        } else if(data->absolute_xy.bytes[0] == 3 && data->absolute_xy.bytes[1] > 128){
            data->gesture = TAP_FINGER_ONE_RIGHT;
        } else {
            data->gesture = TAP_FINGER_ONE_CENTER;
        }
        tapped = true;
    } else if (timer_elapsed32(tap_time) > TAP_TERM && data->ges_evnet1 == 1) {
        data->gesture = TAP_FINGER_TWO;
        tapped = true;
    } else if(can_drag && !use_drag && data->ges_evnet0 == 2) {
        if(!drag_strength_mode && drag_time == 0) {
            drag_time = timer_read32();
        } else if(
            (!drag_strength_mode && timer_elapsed32(drag_time) > drag_term) || 
            (drag_strength_mode && data->touch_strenght1 >= drag_strength)
            ){
            drv2605l_pulse(hf_waveform_number); 
            use_drag = true;
            drag_time = 0;
        }
    } else if(data->touch_strenght3 < 255 && data->ges_evnet1 == 0) {
        tapped3_cnt = tapped3_cnt + 1;
    } 

    if(tapped || data->gesture == TAP_FINGER_THREE){
        tap_time = timer_read32();
    }
    
    if (use_drag){
        rep_mouse->buttons |=  1;
    }

    if(data->finger_cnt == 0){
        drag_time = 0;
        tapped3_cnt = 0;
    }   
}

void set_gesture(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {
    uint32_t dx = ((data->relative_xy.bytes[1] - data->relative_xy.bytes[0]) * default_speed) * accel_speed;
    uint32_t dy = ((data->relative_xy.bytes[3] - data->relative_xy.bytes[2]) * default_speed) * accel_speed;
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

            if(data->gesture == GESTURE_SWIPE_D || data->gesture == GESTURE_SWIPE_U){
                swipe_time = timer_read32();
            }
            gesture_time = timer_read32();
            tapped3_cnt = 0;
        } else if(timer_elapsed32(scroll_time) > scroll_term && data->ges_evnet1 == 2) {
            int scrolling_direction = 0;
            if(data->relative_xy.bytes[2] > 1 && data->relative_xy.bytes[3] > 1 ){
                scrolling_direction = can_reverse_scrolling_direction ? 1 : -1;
            } else if(data->relative_xy.bytes[3] > 1 ){
                scrolling_direction = can_reverse_scrolling_direction ? -1 : 1;
            }
            if(scrolling_direction != 0){
                rep_mouse->v = (scroll_step * accel_step) * scrolling_direction;
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
        if(use_trackpad_layer && can_trackpad_layer){
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
    //debug
    //uprintf("finger: %d iqs5xx_data.gesture: %d\n", data->finger_cnt, data->gesture);

    set_trackpad_layer(data);
    set_gesture(data, rep_mouse);

    if(!data->gesture) {
        set_tap(data, rep_mouse);
    }
}

