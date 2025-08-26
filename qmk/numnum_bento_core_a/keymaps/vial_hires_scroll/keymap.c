/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "device/iqs5xx.h"
#include "drivers/haptic/drv2605l.h"
#include "config/trackpad_config.h"
#include "config/device_config.h"
#include "config/pomodoro_config.h"
#include "timer/pomodoro.h"
#include <math.h>

int end_layer = 6;
int lasted_layer;
int finger_cnt = 0;

keypos_t get_d_3 = (keypos_t){.row = 1, .col = 0};
keypos_t get_u_3 = (keypos_t){.row = 1, .col = 1};
keypos_t get_r_2 = (keypos_t){.row = 1, .col = 2};
keypos_t get_r_3 = (keypos_t){.row = 1, .col = 3};
keypos_t get_l_2 = (keypos_t){.row = 1, .col = 4};
keypos_t get_l_3 = (keypos_t){.row = 2, .col = 0};
keypos_t get_i_2 = (keypos_t){.row = 2, .col = 1};
keypos_t get_o_2 = (keypos_t){.row = 2, .col = 2};
keypos_t get_t_3 = (keypos_t){.row = 2, .col = 3};
keypos_t get_t_1_l = (keypos_t){.row = 3, .col = 0};
keypos_t get_t_1_c = (keypos_t){.row = 3, .col = 1};
keypos_t get_t_1_r = (keypos_t){.row = 3, .col = 2};
keypos_t get_t_2 = (keypos_t){.row = 3, .col = 3};

void gesture_press_key_pressed(keypos_t k, bool pressed) {
    keyevent_t k_event = { .key = k, .type = KEY_EVENT };
    k_event.pressed = pressed;
    k_event.time = (timer_read() | 1);
    action_exec(k_event);   
}

void gesture_press_key(keypos_t k) {
    gesture_press_key_pressed(k, true);
    gesture_press_key_pressed(k, false);
}

enum {
  U_Layer_UP = QK_KB_0,
  U_Layer_DOWN,
  U_M_ACL_2x,
  U_M_ACL_4x,
  U_M_ACL_HALF,
  U_M_ACL_QUARTER,
  U_S_ACL_2x,
  U_S_ACL_4x,
  U_S_ACL_8x,
  U_POMODR_TGL,
  U_EEP_CLR,
  U_H_SCROLL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    U_M_ACL_2x,    U_POMODR_TGL, U_H_SCROLL, 
    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_BTN1, KC_BTN1, KC_BTN1, KC_BTN2        
  ),
  [1] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [2] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [3] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [4] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [5] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [6] = LAYOUT(
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_BTN1, KC_BTN2, QK_BOOT, KC_TRNS, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  )
};

void layer_up(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == end_layer) {
    layer_clear();
  } else {
    layer_move(layer + 1); 
  }
} 

void layer_down(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == 0) {
    layer_move(end_layer);
  } else {
    layer_move(layer - 1); 
  }
}  

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case KC_BTN1: {
      use_drag = record->event.pressed;
      return true; 
      }
    case U_Layer_UP:
      if (record->event.pressed) {    
        layer_up();
      }
      return false;   
    case U_Layer_DOWN:
      if (record->event.pressed) {    
        layer_down();
      }
      return false;   
    case U_M_ACL_2x:
      if (record->event.pressed) {
        accel_speed = accel_speed == 2 ? 1 : 2;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false;  
    case U_M_ACL_4x:
      if (record->event.pressed) {
        accel_speed = accel_speed == 4 ? 1 : 4;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false;  
    case U_M_ACL_HALF:
      if (record->event.pressed) {
        accel_speed = accel_speed == 0.5 ? 1 : 0.5;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);        
      }
      return false;  
    case U_M_ACL_QUARTER:
      if (record->event.pressed) {
        accel_speed = accel_speed == 0.25 ? 1 : 0.25;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false;  
    case U_S_ACL_2x:
      if (record->event.pressed) {
        accel_step = accel_step == 2 ? 1 : 2;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false;  
    case U_S_ACL_4x:
      if (record->event.pressed) {
        accel_step = accel_step == 4 ? 1 : 4;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false; 
    case U_S_ACL_8x:
      if (record->event.pressed) {
        accel_step = accel_step == 8 ? 1 : 8;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false; 
    case U_POMODR_TGL:
      if (record->event.pressed) {
        pomodoro_toggle();
      }
      return false;
    case U_EEP_CLR:
      if (record->event.pressed) {
        eeconfig_init();
        init_device_config(&device_config);
        soft_reset_keyboard();
      }
      return false;
    case U_H_SCROLL:
      if (record->event.pressed) {
        use_horizontal_scrolling = !use_horizontal_scrolling;
        drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
      }
      return false;
    default:
      return true;
  }
}

//haptic
bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    return false;
}

//funkyswtich
bool center_push_holding = false;
keypos_t center_push = {.row = 2, .col = 4}; 

bool in_center_push_press(void) {
  return matrix_is_on(0, 4) &&
    !matrix_is_on(0, 0) && 
    !matrix_is_on(0, 1) && 
    !matrix_is_on(0, 2) && 
    !matrix_is_on(0, 3);
}

void center_push_press(keypos_t k, bool *center_push_holding, bool in_center_push_press) {
  keyevent_t k_event = { .key = k, .type = KEY_EVENT };

  if(!*center_push_holding && in_center_push_press){
    k_event.pressed = true;
    k_event.time = timer_read();
    action_exec(k_event);   
    *center_push_holding = true;
  } else if(*center_push_holding && !in_center_push_press){ 
    k_event.pressed = false;
    k_event.time = timer_read();
    action_exec(k_event);
    *center_push_holding = false;
  }
}

//kb
void keyboard_post_init_user(void) {
  eeconfig_read_user_datablock(&device_config, 0, sizeof(device_config));
  init_iqs5xx();
  wait_ms(300);

  if(!device_config.init) {
    init_device_config(&device_config);
    drv2605l_pulse(trackpad_config.hf_waveform_number);
  }

  set_trackpad_config(device_config.trackpad_config);
  set_pomodoro_config(device_config.pomodoro_config);
  set_led_config(device_config.led_config);
}

void send_pointing_device_kb(report_mouse_t rep_mouse){
    if(rep_mouse.x || rep_mouse.y  || rep_mouse.v || rep_mouse.h || rep_mouse.buttons || clear_buttons){
        pointing_device_set_report(rep_mouse);
        pointing_device_send();
        if(clear_buttons) {
            clear_buttons = false;
        }
    }
}

void matrix_scan_user() {
    iqs5xx_data_t iqs5xx_data;
    iqs5xx_data.gesture = 0;
    report_mouse_t mouse_rep = {0};
    bool is_read_iqs5xx = false;

    is_read_iqs5xx = read_iqs5xx(&iqs5xx_data);
    if (is_read_iqs5xx) {
        process_iqs5xx(&iqs5xx_data, &mouse_rep);

        switch (iqs5xx_data.gesture) {
            case GESTURE_SWIPE_L:
                if(iqs5xx_data.finger_cnt == 2){
                    gesture_press_key(get_l_2);
                } else if(iqs5xx_data.finger_cnt == 3){
                    gesture_press_key(get_l_3);
                }
                break;
            case GESTURE_SWIPE_R:
                if(iqs5xx_data.finger_cnt == 2){
                    gesture_press_key(get_r_2);
                } else if(iqs5xx_data.finger_cnt == 3){
                    gesture_press_key(get_r_3);
                }
                break;
            case GESTURE_SWIPE_D:
                if(iqs5xx_data.finger_cnt == 3){
                  gesture_press_key(get_d_3);
                }
                break;                
            case GESTURE_SWIPE_U:
                if(iqs5xx_data.finger_cnt == 3){
                  gesture_press_key(get_u_3);
                }
                break;
            case GESTURE_PINCH_OUT:
                gesture_press_key(get_o_2);
                break;
            case GESTURE_PINCH_IN:
                gesture_press_key(get_i_2);
                break;
            case TAP_FINGER_ONE_LEFT:
                gesture_press_key(get_t_1_l);
                break;
            case TAP_FINGER_ONE_CENTER:
                gesture_press_key(get_t_1_c);
                break;
            case TAP_FINGER_ONE_RIGHT:
                gesture_press_key(get_t_1_r);
                break;
            case TAP_FINGER_TWO:
                gesture_press_key(get_t_2);
                break;
            case TAP_FINGER_THREE:
                gesture_press_key(get_t_3);
            default:
                break;
        }
        send_pointing_device_kb(mouse_rep);
    }

  center_push_press(center_push, &center_push_holding, in_center_push_press());
  
  if (timer_active) {
    pomodoro_update();
  }
  
  check_and_save_device_config();

  int current_layer = get_highest_layer(layer_state|default_layer_state); 

  if(trackpad_config.can_hf_for_layer && lasted_layer != current_layer){
    if(trackpad_config.can_trackpad_layer &&
       current_layer != trackpad_layer && lasted_layer != trackpad_layer
    ){
      drv2605l_pulse(trackpad_config.hf_waveform_number);
    } else if(!trackpad_config.can_trackpad_layer) {
      drv2605l_pulse(trackpad_config.hf_waveform_number);
    }
    lasted_layer = current_layer;
  }
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [1] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [2] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [3] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [4] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [5] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [6] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) }
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_layer = get_highest_layer(layer_state|default_layer_state);  
    int rgb_matrix_mode = rgb_matrix_get_mode();

    RGB rgb;
    if (current_layer < device_config.led_config.layer_count) {
        rgb.r = device_config.led_config.layer_colors[current_layer].r;
        rgb.g = device_config.led_config.layer_colors[current_layer].g;
        rgb.b = device_config.led_config.layer_colors[current_layer].b;
    } else {
        rgb.r = device_config.led_config.layer_colors[0].r;
        rgb.g = device_config.led_config.layer_colors[0].g;
        rgb.b = device_config.led_config.layer_colors[0].b;
    }
  for (uint8_t i = led_min; i <= led_max; i++) {
    if(rgb_matrix_mode == 1 && HAS_FLAGS(g_led_config.flags[i], 0x04)) {
      rgb_matrix_set_color(i, 0, 0, 0);
    } else if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } 
  }
  if(rgb_matrix_mode != 1){ return false; }

  RGB rgb_speed = {
    device_config.led_config.indicator_colors.speed_r,
    device_config.led_config.indicator_colors.speed_g,
    device_config.led_config.indicator_colors.speed_b
  };
  if (accel_speed == 0.25) {
    rgb_matrix_set_color(6, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(7, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 0.5) {
    rgb_matrix_set_color(6, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(7, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 1) {
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 2) {
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 4) {
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(10, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  }

  RGB rgb_step = {
    device_config.led_config.indicator_colors.step_r,
    device_config.led_config.indicator_colors.step_g,
    device_config.led_config.indicator_colors.step_b
  };
  if (accel_step == 1) {
    rgb_matrix_set_color(11, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 2) {
    rgb_matrix_set_color(11, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(12, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 4) {
    rgb_matrix_set_color(11, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(12, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(13, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 8) {
    rgb_matrix_set_color(11, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(12, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(13, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(14, rgb_step.r, rgb_step.g, rgb_step.b);
  }

    const uint8_t h_scroll_leds[] = {16, 17};

  RGB rgb_h_scroll = {
    device_config.led_config.indicator_colors.h_scroll_r,
    device_config.led_config.indicator_colors.h_scroll_g,
    device_config.led_config.indicator_colors.h_scroll_b
  };
  if(use_horizontal_scrolling){
    for (uint8_t i = 0; i < sizeof(h_scroll_leds); i++) {
      rgb_matrix_set_color(h_scroll_leds[i], rgb_h_scroll.r, rgb_h_scroll.g, rgb_h_scroll.b);
    }  
  }

  const uint8_t pomodoro_leds[] = {18};
  
  // Check if we're flashing due to color change
  if (is_pomodoro_flashing()) {
    RGB flash_color = get_pomodoro_flash_color();
    // Flash only pomodoro LEDs with the changed color
    for (uint8_t i = 0; i < sizeof(pomodoro_leds); i++) {
      rgb_matrix_set_color(pomodoro_leds[i], flash_color.r, flash_color.g, flash_color.b);
    }
  } else if(phase != POMODORO_IDLE){
    // Normal pomodoro timer display
    RGB color = {0, 0, 0};
    switch (phase) {
      case POMODORO_WORK:
        color.r = device_config.led_config.pomodoro_colors.work_r;
        color.g = device_config.led_config.pomodoro_colors.work_g;
        color.b = device_config.led_config.pomodoro_colors.work_b;
        break;
      case POMODORO_BREAK:
        color.r = device_config.led_config.pomodoro_colors.break_r;
        color.g = device_config.led_config.pomodoro_colors.break_g;
        color.b = device_config.led_config.pomodoro_colors.break_b;
        break;
      case POMODORO_LONG_BREAK:
        color.r = device_config.led_config.pomodoro_colors.long_break_r;
        color.g = device_config.led_config.pomodoro_colors.long_break_g;
        color.b = device_config.led_config.pomodoro_colors.long_break_b;
        break;
      default:
        break;
    }
    
    // Set all pomodoro LEDs to the same color
    for (uint8_t i = 0; i < sizeof(pomodoro_leds); i++) {
      rgb_matrix_set_color(pomodoro_leds[i], color.r, color.g, color.b);
    }
  }
  return false;
};