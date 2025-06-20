/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "i2c_master.h"
#include "iqs5xx.h"
#include "quantum.h"
#include "drivers/haptic/drv2605l.h"
#include "../timer/pomodoro.h"
#include "quantum/keyboard.h"

// Global state variables
uint16_t iqs_device_addr = IQS5xx_DEVICE_ADDR<<1;
bool clear_buttons = false;

// Internal state variables - only used within this file
static uint32_t tap_time;
static uint32_t swipe_time;
static uint32_t pinch_time;
static uint32_t gesture_time;
static uint32_t scroll_time;
static uint32_t short_scroll_timer;
static uint32_t drag_time;
static bool tapped;
static int tapped2_cnt;
static int tapped3_cnt;
static int scrolling_direction = 1;
static bool scroll_start = false;
static bool scroll_end = false;

static inline uint8_t iqs_app_readReg_continue(uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_read_register16(iqs_device_addr, regaddr, data, len, 100);
}

static inline uint8_t iqs_app_end_communication(void) {
    uint8_t _ = 0xFF;
    return i2c_write_register16(iqs_device_addr, IQS5xx_COM_END_REG, &_, 1, 100);
}

static inline uint8_t iqs_app_writeReg(uint16_t regaddr, uint8_t* data, uint16_t len) {
    return i2c_write_register16(iqs_device_addr, regaddr, data, len, 100);
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
  tap_time = timer_read32();
  swipe_time = timer_read32();
  pinch_time = timer_read32();
  gesture_time = timer_read32();
  drag_time = 0;
}

bool read_iqs5xx(iqs5xx_data_t* const data) {
  uint8_t i2c_res = iqs_app_readReg_continue(IQS5xx_FINGER_NUM, &data->finger_cnt, 1);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT0, &data->ges_evnet0, 1);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT1, &data->ges_evnet1, 1);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_RELATIVE_XY, (uint8_t*)&data->relative_xy, 4);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_ABSOLUTE_XY, (uint8_t*)&data->absolute_xy, 4);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_TOUCH_STRENGTH_FINGER1, (uint8_t*)&data->touch_strenght1, 1);

  bool res = i2c_res == 0;
  if (res) {
    iqs_app_end_communication();
  }
  return res;
}

bool is_inactive_area(iqs5xx_data_t* const data) {  
    uint16_t y = (data->absolute_xy.bytes[3] << 8) | data->absolute_xy.bytes[2];
    return data->absolute_xy.bytes[2] != 3 || (data->absolute_xy.bytes[2] == 3 && y < MISS_TAP_THRESHOLD);
}  

void set_tap(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse, bool is_inactive_area) {    
    // If we're in a break and waiting for double-tap to cancel vibration, don't process regular taps
    if (waiting_for_break_touch) {
        return;
    }
    
    if(tapped && data->finger_cnt == 0){
        rep_mouse->buttons = 0;
        clear_buttons = true;
        tapped = false;
        use_drag = false;
        return;  
    }

    if(data->ges_evnet1 == 0 && data->finger_cnt == 2) {
        tapped2_cnt = tapped2_cnt + 1;
    } 

    if(data->ges_evnet1 == 0 && data->finger_cnt == 3) {
        tapped3_cnt = tapped3_cnt + 1;
    } 

    if (data->ges_evnet0 == 1 && is_inactive_area) {          
        if(data->absolute_xy.bytes[0] == 0 && data->absolute_xy.bytes[1] < 128){
            data->gesture = TAP_FINGER_ONE_LEFT;
        } else if(data->absolute_xy.bytes[0] == 3 && data->absolute_xy.bytes[1] > 128){
            data->gesture = TAP_FINGER_ONE_RIGHT;
        } else {
            data->gesture = TAP_FINGER_ONE_CENTER;
        }
        tapped = true;
    } else if(timer_elapsed32(tap_time) > trackpad_config.tap_term && tapped2_cnt > 2 && data->ges_evnet1 == 1) {
        data->gesture = TAP_FINGER_TWO;
        tapped = true;
    } else if(timer_elapsed32(tap_time) > trackpad_config.tap_term && tapped3_cnt > 2 && data->ges_evnet1 == 1){
        data->gesture = TAP_FINGER_THREE;
        tapped = true;
    } else if(trackpad_config.can_drag && !use_drag && data->ges_evnet0 == 2) {
        if(!trackpad_config.drag_strength_mode && drag_time == 0) {
            drag_time = timer_read32();
        } else if(is_inactive_area && (
            (!trackpad_config.drag_strength_mode && timer_elapsed32(drag_time) > trackpad_config.drag_term) || 
            (trackpad_config.drag_strength_mode && data->touch_strenght1 >= trackpad_config.drag_strength)
            )){
            drv2605l_pulse(trackpad_config.hf_waveform_number);
            use_drag = true;
            drag_time = 0;
        }
    }

    if(tapped || data->gesture == TAP_FINGER_THREE){
        tap_time = timer_read32();
    }
    
    if (use_drag){
        rep_mouse->buttons |=  1;
    }

    if(data->finger_cnt == 0){
        drag_time = 0;
    }   

    if(data->ges_evnet1 > 0){
        tapped2_cnt = 0;
        tapped3_cnt = 0;
    }   
}

int32_t move_limit_range(int32_t value, int32_t min, int32_t max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

void set_gesture(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse, bool is_inactive_area) {
    double _default_speed = (double)trackpad_config.default_speed / 10.0;
    int32_t dx = ((data->relative_xy.bytes[1] - data->relative_xy.bytes[0]) * _default_speed) * accel_speed;
    int32_t dy = ((data->relative_xy.bytes[3] - data->relative_xy.bytes[2]) * _default_speed) * accel_speed;

    dx = abs(dx) < MIN_MOVE_THRESHOLD ? 0 : dx;
    dy = abs(dy) < MIN_MOVE_THRESHOLD ? 0 : dy;

    dx = move_limit_range(dx, -MAX_SPEED, MAX_SPEED);
    dy = move_limit_range(dy, -MAX_SPEED, MAX_SPEED);

    if (data->finger_cnt == 1 && is_inactive_area) {
        rep_mouse->x = dx;
        rep_mouse->y = dy;
    } else if (data->finger_cnt >= 2) {
        if(timer_elapsed32(swipe_time) > trackpad_config.swipe_term && data->ges_evnet1 == 2 && data->finger_cnt == 2) {
            if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_L;
            } else if(data->relative_xy.bytes[1] > 0){
                data->gesture = GESTURE_SWIPE_R;
            } 
            swipe_time = timer_read32();
        } else if(timer_elapsed32(gesture_time) > trackpad_config.gesture_term && data->ges_evnet1 == 2 && data->finger_cnt == 3) {
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
        } else if(data->ges_evnet1 == 2 && data->relative_xy.bytes[1] == 0) {
            if(!scroll_start){
                scroll_start = true;
                short_scroll_timer = timer_read32();
            }

            if(data->relative_xy.bytes[2] > 1 && data->relative_xy.bytes[3] > 1 ){
                    scrolling_direction = trackpad_config.can_reverse_scrolling_direction ? 1 : -1;
            } else if(data->relative_xy.bytes[3] > 1 ){
                    scrolling_direction = trackpad_config.can_reverse_scrolling_direction ? -1 : 1;
            }
            
           if(timer_elapsed32(scroll_time) > trackpad_config.scroll_term){
                rep_mouse->v = (trackpad_config.scroll_step + 1) * accel_step * scrolling_direction;
                scroll_time = timer_read32();
            }
        } else if(timer_elapsed32(pinch_time) > trackpad_config.pinch_term && data->ges_evnet1 == 4) {
            if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0) {
                data->gesture = GESTURE_PINCH_OUT;
            } else if(data->relative_xy.bytes[1] > 0) {
                data->gesture = GESTURE_PINCH_IN;
            }
            pinch_time = timer_read32();
        }
    }
 
    if(scroll_start && data->finger_cnt == 0){
        scroll_end = true; 
    }

    if(trackpad_config.can_short_scroll && scroll_end){
        if(timer_elapsed32(short_scroll_timer) < trackpad_config.short_scroll_term){
            int base_scroll = 3;
            float current_scroll = base_scroll + (trackpad_config.scroll_step + 1);
            float decay_step = 0.5;
            while (current_scroll > 0) {
                rep_mouse->v = current_scroll * accel_step * scrolling_direction;
                pointing_device_set_report(*rep_mouse);
                pointing_device_send();

                current_scroll -= decay_step;
                if (current_scroll < 0) {
                    current_scroll = 0;
                }
                wait_ms(10);
            }
        }
        short_scroll_timer = timer_read32();
        scroll_start = false;
        scroll_end = false;
    }
}

void set_trackpad_layer(iqs5xx_data_t* const data, bool is_inactive_area) {
  if (data->finger_cnt == 0) {
    if(use_trackpad_layer && trackpad_config.can_trackpad_layer){
      layer_move(get_highest_layer(default_layer_state));            
      use_trackpad_layer = false;
    }
  } else {
    if(!use_trackpad_layer && trackpad_config.can_trackpad_layer && is_inactive_area){
      layer_move(trackpad_layer);
      use_trackpad_layer = true;
    }
  }
}

void process_iqs5xx(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {
  if(data->finger_cnt > 0 && last_input_activity_elapsed() > (uint32_t)RGB_MATRIX_TIMEOUT) {
    last_matrix_activity_trigger();
  }

  pomodoro_process_touch(data->finger_cnt);
  bool _is_inactive_area = is_inactive_area(data);
  set_trackpad_layer(data, _is_inactive_area);
  set_tap(data, rep_mouse, _is_inactive_area);
  set_gesture(data, rep_mouse, _is_inactive_area);
}

void set_iqs5xx_zoom_distance(uint16_t distance) {
    uint8_t distanceArray[2];
    distanceArray[0] = (distance >> 8) & 0xFF;
    distanceArray[1] = distance & 0xFF;
    iqs_app_writeRegR(IQS5xx_ZOOM, distanceArray, 2);
}
