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
bool is_double_tap_detected = false;
bool is_trackpad_press = false;
bool tapped;
iqs_timer_t timer = {0};
bool is_tap_center_tap = false;
bool is_tap_left_tap = false;
bool is_tap_right_tap = false;

// internal configuration variables
static int scrolling_direction = 1;

// Double tap detection variables  
static bool is_waiting_second_tap = false;
static uint8_t last_finger_count = 0;
static uint8_t last_tapped_finger_count = 0;

// Gesture state tracking
typedef struct {
  int tapped2_cnt;
  int tapped3_cnt;
  int tapped4_cnt;
  bool scroll_start;
} iqs_gesture_state_t;

static iqs_gesture_state_t gesture_state = {.tapped2_cnt = 0,
                                            .tapped3_cnt = 0,
                                            .tapped4_cnt = 0,
                                            .scroll_start = false};

static inline uint8_t iqs_app_readReg_continue(uint16_t regaddr, uint8_t* data,
                                               uint16_t len) {
  return i2c_read_register16(iqs_device_addr, regaddr, data, len,
                       I2C_RETRY_INTERVAL_MS);
}

static inline uint8_t iqs_app_end_communication(void) {
  uint8_t _ = 0xFF;
  return i2c_write_register16(iqs_device_addr, IQS5xx_COM_END_REG, &_, 1,
                        I2C_RETRY_INTERVAL_MS);
}

static inline uint8_t iqs_app_writeReg(uint16_t regaddr, uint8_t* data,
                                       uint16_t len) {
  return i2c_write_register16(iqs_device_addr, regaddr, data, len,
                        I2C_RETRY_INTERVAL_MS);
}

void iqs_app_writeRegR(uint16_t regaddr, uint8_t* data, uint16_t len) {
  int retry_count = I2C_RETRY_COUNT;
  while (retry_count > 0) {
    uint8_t res = iqs_app_writeReg(regaddr, data, len);
    if (res == I2C_STATUS_SUCCESS) {
      break;
    }
    wait_ms(I2C_RETRY_INTERVAL_MS);
    retry_count--;
  }
}

void init_iqs5xx(void) {
  uint8_t ati_target[] = {0, 200};
  iqs_app_writeRegR(IQS5xx_ATI_TARGET, ati_target, 2);

  timer.tap_time = timer_read32();
  timer.swipe_time = timer_read32();
  timer.pinch_time = timer_read32();
  timer.gesture_time = timer_read32();

}

bool read_iqs5xx(iqs5xx_data_t* const data) {
  uint8_t i2c_res =
      iqs_app_readReg_continue(IQS5xx_FINGER_NUM, &data->finger_cnt, 1);
  i2c_res |=
      iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT0, &data->ges_evnet0, 1);
  i2c_res |=
      iqs_app_readReg_continue(IQS5xx_GESTURE_EVENT1, &data->ges_evnet1, 1);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_RELATIVE_XY,
                                      (uint8_t*)&data->relative_xy, 4);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_ABSOLUTE_XY,
                                      (uint8_t*)&data->absolute_xy, 4);
  i2c_res |= iqs_app_readReg_continue(IQS5xx_TOUCH_STRENGTH_FINGER1,
                                      (uint8_t*)&data->touch_strenght1, 2);

  bool res = i2c_res == 0;
  if (res) {
    iqs_app_end_communication();
  }
  return res;
}

static void update_tap_counters(iqs5xx_data_t* const data) {
  if (data->ges_evnet1 == 0) {
    if (data->finger_cnt == 2) {
      gesture_state.tapped2_cnt++;
    } else if (data->finger_cnt == 3) {
      gesture_state.tapped3_cnt++;
    } else if (data->finger_cnt == 4) {
      gesture_state.tapped4_cnt++;
    }
  }
}

static void handle_tap_area(iqs5xx_data_t* const data) {
  if (data->absolute_xy.bytes[0] == 0 && data->absolute_xy.bytes[2] == 0 &&
      data->absolute_xy.bytes[1] < TAP_EDGE_THRESHOLD && data->absolute_xy.bytes[3] < TAP_EDGE_THRESHOLD) {
    is_tap_center_tap = false;
    is_tap_left_tap = true;
    is_tap_right_tap = false;
  } else if (data->absolute_xy.bytes[0] == 3 && data->absolute_xy.bytes[2] == 0 &&
    data->absolute_xy.bytes[1] > TAP_EDGE_THRESHOLD && data->absolute_xy.bytes[3] < TAP_EDGE_THRESHOLD)  {
    is_tap_center_tap = false;
    is_tap_left_tap = false;
    is_tap_right_tap = true;
  } else {
    is_tap_center_tap = true;
    is_tap_left_tap = false;
    is_tap_right_tap = false;
  }
}

static void handle_single_tap(iqs5xx_data_t* const data) {
  if (!tapped && data->ges_evnet0 == 1) {
    if (is_tap_left_tap) {
      data->gesture = TAP_FINGER_ONE_LEFT;
    } else if (is_tap_right_tap)  {
      data->gesture = TAP_FINGER_ONE_RIGHT;
    } else {
      data->gesture = TAP_FINGER_ONE_CENTER;
    }
    tapped = true;
  }
}

static void handle_multi_tap(iqs5xx_data_t* const data) {
  if (!tapped && timer_elapsed32(timer.tap_time) > trackpad_config.tap_term && data->ges_evnet1 == 1) {
    if (gesture_state.tapped4_cnt > 0) {
      data->gesture = TAP_FINGER_FOUR;
      tapped = true;
    } else if (gesture_state.tapped3_cnt > 0) {
      data->gesture = TAP_FINGER_THREE;
      tapped = true;
    }else if (gesture_state.tapped2_cnt > 0) {
      data->gesture = TAP_FINGER_TWO;
      tapped = true;
    }
  }
}

static void handle_double_tap(iqs5xx_data_t* const data) {   
    // Detect single finger tap event (finger down)
    if (data->finger_cnt > 0 && data->ges_evnet1 == 0) {
      if(last_finger_count == 0) {
        // Within trackpad_config.tap_term - this is a double tap!
        if (is_waiting_second_tap && timer_elapsed32(timer.tap_time) < trackpad_config.tap_term) {
          is_double_tap_detected = true;
          last_tapped_finger_count = data->finger_cnt;
        } 
        if(!is_waiting_second_tap) {
          is_double_tap_detected = false;
          is_waiting_second_tap = true;
          last_tapped_finger_count = data->finger_cnt;
        }
      }
    }
        
    if (timer_elapsed32(timer.tap_time) > trackpad_config.tap_term) {
      is_waiting_second_tap = false;
    } 
    last_finger_count = data->finger_cnt;
}

void set_tap(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {

  //handle tap release
  if (data->finger_cnt == 0) {
    if (tapped) {
      rep_mouse->buttons = 0;
      clear_buttons = true;
      tapped = false;
      use_drag = false;
      gesture_state.tapped2_cnt = 0;
      gesture_state.tapped3_cnt = 0;
      gesture_state.tapped4_cnt = 0;
      return;
    }
  }
    
  handle_tap_area(data);
  update_tap_counters(data);

  if(!is_trackpad_press){
    handle_single_tap(data);
    handle_multi_tap(data);
    handle_double_tap(data);
  }

  if (tapped) {
    timer.tap_time = timer_read32();
  }

  if (use_drag) {
    rep_mouse->buttons |= 1;
  }

  if (data->ges_evnet1 > 0) {
    gesture_state.tapped2_cnt = 0;
    gesture_state.tapped3_cnt = 0;
    gesture_state.tapped4_cnt = 0;
  }
}

int32_t move_limit_range(int32_t value, int32_t min, int32_t max) {
  if (value > max) return max;
  if (value < min) return min;
  return value;
}

static void calculate_cursor_movement(iqs5xx_data_t* const data, int32_t* dx,
                                      int32_t* dy) {
  double _default_speed = (double)trackpad_config.default_speed / 10.0;

  *dx = ((data->relative_xy.bytes[1] - data->relative_xy.bytes[0]) *
         _default_speed) *
        accel_speed;
  *dy = ((data->relative_xy.bytes[3] - data->relative_xy.bytes[2]) *
         _default_speed) *
        accel_speed;

  *dx = abs(*dx) < MIN_MOVE_THRESHOLD ? 0 : *dx;
  *dy = abs(*dy) < MIN_MOVE_THRESHOLD ? 0 : *dy;
  
  *dx = move_limit_range(*dx, -MAX_SPEED, MAX_SPEED);
  *dy = move_limit_range(*dy, -MAX_SPEED, MAX_SPEED);
}

static bool is_gesture_finger_swipe(const iqs5xx_data_t* data) {
    return (timer_elapsed32(timer.swipe_time) > trackpad_config.swipe_term && data->finger_cnt > 1 && data->ges_evnet1 == 2);
}

static void handle_finger_swipe(iqs5xx_data_t* const data) {
  if (!is_gesture_finger_swipe(data)) return;

  if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0){
    data->gesture = GESTURE_SWIPE_L;
  } else if(data->relative_xy.bytes[1] > 0){
    data->gesture = GESTURE_SWIPE_R;
  } else if(data->finger_cnt > 2 && data->relative_xy.bytes[2] > 0 && data->relative_xy.bytes[3] > 0 ){
    data->gesture = GESTURE_SWIPE_D;
  } else if(data->finger_cnt > 2 && data->relative_xy.bytes[3] > 0 ){
    data->gesture = GESTURE_SWIPE_U;
  }
  timer.swipe_time = timer_read32();
}

static void handle_scroll(iqs5xx_data_t* const data,
                          report_mouse_t* const rep_mouse) {

  if (data->ges_evnet1 == 2 && data->relative_xy.bytes[1] == 0) {
    
    if(data->relative_xy.bytes[2] > 1 && data->relative_xy.bytes[3] > 1 ){
      scrolling_direction = trackpad_config.can_reverse_scrolling_direction ? 1 : -1;
    } else if(data->relative_xy.bytes[3] > 1 ){
      scrolling_direction = trackpad_config.can_reverse_scrolling_direction ? -1 : 1;
    }
    
    if (!gesture_state.scroll_start) {
      gesture_state.scroll_start = true;
      timer.short_scroll_term = timer_read32();
    }

    if (timer_elapsed32(timer.scroll_time) > trackpad_config.scroll_term) {
      rep_mouse->v = (trackpad_config.scroll_step + 1) * accel_step * scrolling_direction;
      wait_ms(SCROLL_INTERVAL_MS);
      rep_mouse->v = (trackpad_config.scroll_step + 1) * accel_step * scrolling_direction;

      timer.scroll_time = timer_read32();
    }
  }
}

static void handle_short_scroll(iqs5xx_data_t* const data,
                                report_mouse_t* const rep_mouse) {
  if (trackpad_config.can_short_scroll && gesture_state.scroll_start) {
    if (timer_elapsed32(timer.short_scroll_term) < trackpad_config.short_scroll_term) {
      for (int i = 0; i < SHORT_SCROLL_REPEAT; i++) {
        rep_mouse->v = (trackpad_config.scroll_step + 1) * accel_step * scrolling_direction;

        pointing_device_set_report(*rep_mouse);
        pointing_device_send();
        wait_ms(SCROLL_INTERVAL_MS);
      }
    }
    timer.short_scroll_term = timer_read32();
    gesture_state.scroll_start = false;
  }
}

static void handle_pinch(iqs5xx_data_t* const data) {
  if (timer_elapsed32(timer.pinch_time) > trackpad_config.pinch_term && data->ges_evnet1 == 4) {
    
    if(data->relative_xy.bytes[0] > 0 && data->relative_xy.bytes[1] > 0) {
      data->gesture = GESTURE_PINCH_OUT;
    } else if(data->relative_xy.bytes[1] > 0) {
      data->gesture = GESTURE_PINCH_IN;
    }

    timer.pinch_time = timer_read32();
  }
}

void set_gesture(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse) {
  int32_t dx, dy;
  calculate_cursor_movement(data, &dx, &dy);

  if (data->finger_cnt == 0) {
    handle_short_scroll(data, rep_mouse);
  } else if (data->finger_cnt == 1) {
    rep_mouse->x = dx;
    rep_mouse->y = dy;
  } else if (data->finger_cnt == 2) {
    handle_scroll(data, rep_mouse);
    handle_finger_swipe(data);
    handle_pinch(data);
  } else if (data->finger_cnt == 3) {
    handle_finger_swipe(data);
  } else if (data->finger_cnt == 4) {
    handle_finger_swipe(data);
  }
}

void set_trackpad_layer(iqs5xx_data_t* const data) {
  if (data->finger_cnt == 0) {
    if (use_trackpad_layer && trackpad_config.can_trackpad_layer) {
      layer_move(get_highest_layer(default_layer_state));
      use_trackpad_layer = false;
    }
  } else {
    if (!use_trackpad_layer && trackpad_config.can_trackpad_layer) {
      layer_move(trackpad_layer);
      use_trackpad_layer = true;
    }
  }
}

void set_iqs5xx_zoom_distance(uint16_t distance) {
    uint8_t distanceArray[2];
    distanceArray[0] = (distance >> 8) & 0xFF;
    distanceArray[1] = distance & 0xFF;
    iqs_app_writeRegR(IQS5xx_ZOOM, distanceArray, 2);
}

void process_iqs5xx(iqs5xx_data_t* const data,
                    report_mouse_t* const rep_mouse) {
    if(data->finger_cnt > 0 && last_input_activity_elapsed() > (uint32_t)RGB_MATRIX_TIMEOUT) {
        last_matrix_activity_trigger();
    }
    set_trackpad_layer(data);
    set_tap(data, rep_mouse);
    set_gesture(data, rep_mouse);
}
