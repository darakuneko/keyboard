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

keypos_t get_d_3 = (keypos_t){.row = 4, .col = 0};
keypos_t get_u_3 = (keypos_t){.row = 4, .col = 1};
keypos_t get_r_2 = (keypos_t){.row = 4, .col = 2};
keypos_t get_r_3 = (keypos_t){.row = 4, .col = 3};
keypos_t get_l_2 = (keypos_t){.row = 4, .col = 4};
keypos_t get_l_3 = (keypos_t){.row = 4, .col = 5};
keypos_t get_i_2 = (keypos_t){.row = 4, .col = 6};
keypos_t get_o_2 = (keypos_t){.row = 4, .col = 7};
keypos_t get_t_3 = (keypos_t){.row = 4, .col = 8};
keypos_t get_t_1_l = (keypos_t){.row = 4, .col = 9};
keypos_t get_t_1_c = (keypos_t){.row = 4, .col = 10};
keypos_t get_t_1_r = (keypos_t){.row = 5, .col = 0};
keypos_t get_t_2 = (keypos_t){.row = 5, .col = 1};

void gesture_press_key(keypos_t k) {
    keyevent_t k_event = { .key = k, .type = KEY_EVENT };
    k_event.pressed = true;
    k_event.time = (timer_read() | 1);
    action_exec(k_event);   
    k_event.pressed = false;
    k_event.time = (timer_read() | 1);
    action_exec(k_event);
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
  U_EEP_CLR
};

int end_layer = 4;
int lasted_layer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	
  [0] = LAYOUT(
	  KC_ESC,              QK_BOOT,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
		MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
		           KC_MNXT,           KC_RALT, LT(1, KC_SPC),LT(1, KC_BSPC),MO(2),  
    KC_VOLU,   KC_MPLY,   KC_MPRV,   
               KC_VOLD,

    LGUI(KC_TAB), LGUI(KC_TAB),
    LALT(KC_RGHT), LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS), LCTL(KC_PMNS),
    KC_ESC, KC_BTN1, KC_BTN1, KC_BTN1, KC_BTN2                    
  ),

	[1] = LAYOUT(
		KC_GRV,    KC_MINS, KC_EQL,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
		KC_LGUI,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,
		KC_LSFT,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LBRC, KC_RBRC, KC_SCLN, KC_QUOT, KC_SLSH,
		KC_NO,   KC_NO,   KC_LSFT, KC_LSFT, KC_NO,
	
    KC_NO,   KC_NO,   KC_NO,   KC_NO, 

    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,  
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	[2] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,

    KC_NO,   KC_NO,   KC_NO,   KC_NO, 

    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

  [3] = LAYOUT(
	  KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
		MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
				       KC_MNXT,           KC_RALT, LT(4, KC_SPC),LT(4, KC_BSPC),MO(5),  
    KC_VOLU,   KC_MPLY,   KC_MPRV,   
               KC_VOLD,

    LGUI(KC_TAB), LGUI(KC_TAB),
    LALT(KC_RGHT), LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS), LCTL(KC_PMNS),
    KC_ESC, KC_BTN1, KC_BTN1, KC_BTN1, KC_BTN2                    
  ),

	[4] = LAYOUT(
		KC_GRV,    KC_MINS, KC_EQL,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, KC_NO,
		KC_LGUI,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,
		KC_LSFT,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LBRC, KC_RBRC, KC_SCLN, KC_QUOT, KC_SLSH,
		KC_NO,   KC_NO,   KC_LSFT, KC_LSFT, KC_NO,
	
    KC_NO,   KC_NO,   KC_NO,   KC_NO, 

    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,  
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),
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

keypos_t center_push = {.row = 3, .col = 0}; 
bool in_center_push_press(void) {
  return matrix_is_on(0, 0) &&
    matrix_is_on(1, 0) && 
    matrix_is_on(2, 0) && 
    matrix_is_on(3, 0) && 
    matrix_is_on(4, 0) && 
    matrix_is_on(5, 0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if(in_center_push_press() && 
  record->event.key.col == center_push.col &&
  record->event.key.row != center_push.row        
  ){ return false; }
  
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
      }
      return false;  
    case U_M_ACL_4x:
      if (record->event.pressed) {
        accel_speed = accel_speed == 4 ? 1 : 4;
      }
      return false;  
    case U_M_ACL_HALF:
      if (record->event.pressed) {
        accel_speed = accel_speed == 0.5 ? 1 : 0.5;
      }
      return false;  
    case U_M_ACL_QUARTER:
      if (record->event.pressed) {
        accel_speed = accel_speed == 0.25 ? 1 : 0.25;
      }
      return false;  
    case U_S_ACL_2x:
      if (record->event.pressed) {
        accel_step = accel_step == 2 ? 1 : 2;
      }
      return false;  
    case U_S_ACL_4x:
      if (record->event.pressed) {
        accel_step = accel_step == 4 ? 1 : 4;
      }
      return false; 
    case U_S_ACL_8x:
      if (record->event.pressed) {
        accel_step = accel_step == 8 ? 1 : 8;
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
        soft_reset_keyboard();
        init_device_config(&device_config);
      }
      return false;
    default:
      return true;
  }
}

void keyboard_post_init_user(void) {
  eeconfig_read_user_datablock(&device_config);
  init_iqs5xx();
  wait_ms(300);

  if(!device_config.init) {
    init_device_config(&device_config);
  }

  set_trackpad_config(device_config.trackpad_config);
  set_pomodoro_config(device_config.pomodoro_config);  
}

void send_pointing_device_kb(report_mouse_t rep_mouse){
  if(rep_mouse.x || rep_mouse.y  || rep_mouse.v || rep_mouse.buttons || clear_buttons){
    pointing_device_set_report(rep_mouse);
    pointing_device_send();
    if(clear_buttons) {
      clear_buttons = false;
    }
  }
}

void matrix_scan_user(void) {
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
        gesture_press_key(get_d_3);
        break;                
      case GESTURE_SWIPE_U:
        gesture_press_key(get_u_3);
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
        break;
      default:
        break;
    }
    send_pointing_device_kb(mouse_rep);
  }

  if (timer_active) {
    pomodoro_update();
  }

  int current_layer = get_highest_layer(layer_state|default_layer_state); 

  if(can_hf_for_layer && lasted_layer != current_layer){
    if(device_config.trackpad_config.can_trackpad_layer &&
       current_layer != trackpad_layer && lasted_layer != trackpad_layer
    ){
      drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
    } else if(!device_config.trackpad_config.can_trackpad_layer) {
      drv2605l_pulse(device_config.trackpad_config.hf_waveform_number);
    }
    lasted_layer = current_layer;
  }
}

bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    return false;
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(LCTL(KC_KP_MINUS), LCTL(KC_KP_PLUS)) },
    [1] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [2] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [3] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [4] = { ENCODER_CCW_CW(KC_NO, KC_NO) }
};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int current_layer = get_highest_layer(layer_state|default_layer_state);  
  int rgb_matrix_val = rgb_matrix_get_val();
  int rgb_matrix_mode = rgb_matrix_get_mode();
  HSV hsv = {0, 255, rgb_matrix_val};
  if (current_layer == 1) {
    hsv.h = 191; //PURPLE
  } else if (current_layer == 2)  {
    hsv.h = 85; //GREEN
  } else if (current_layer == 3)  {
    hsv.h = 43; //YELLOW
  } else if (current_layer == 4)  {
    hsv.h = 222; //ROSE
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);
  for (uint8_t i = led_min; i <= led_max; i++) {
    if(rgb_matrix_mode == 1 && HAS_FLAGS(g_led_config.flags[i], 0x04)) {
      rgb_matrix_set_color(i, 0, 0, 0);
    } else if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } 
  }

  if(rgb_matrix_mode != 1){ return false; }

  HSV hsv_speed = {255, 255, rgb_matrix_val};
  RGB rgb_speed = hsv_to_rgb(hsv_speed);
  if (accel_speed == 0.25) {
    rgb_matrix_set_color(7, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 0.5) {
    rgb_matrix_set_color(8, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 1) {
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 2) {
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(10, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  } else if (accel_speed == 4) {
    rgb_matrix_set_color(9, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(10, rgb_speed.r, rgb_speed.g, rgb_speed.b);
    rgb_matrix_set_color(11, rgb_speed.r, rgb_speed.g, rgb_speed.b);
  }

  HSV hsv_step = {255, 255, rgb_matrix_val};
  RGB rgb_step = hsv_to_rgb(hsv_step);
  if (accel_step == 1) {
    rgb_matrix_set_color(19, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 2) {
    rgb_matrix_set_color(19, rgb_step.r, rgb_step.g, rgb_step.b);
    rgb_matrix_set_color(20, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 4) {
      rgb_matrix_set_color(19, rgb_step.r, rgb_step.g, rgb_step.b);
      rgb_matrix_set_color(20, rgb_step.r, rgb_step.g, rgb_step.b);
      rgb_matrix_set_color(21, rgb_step.r, rgb_step.g, rgb_step.b);
  } else if (accel_step == 8) {
      rgb_matrix_set_color(19, rgb_step.r, rgb_step.g, rgb_step.b);
      rgb_matrix_set_color(20, rgb_step.r, rgb_step.g, rgb_step.b);
      rgb_matrix_set_color(21, rgb_step.r, rgb_step.g, rgb_step.b);
      rgb_matrix_set_color(22, rgb_step.r, rgb_step.g, rgb_step.b);
  }
  
  if(phase != POMODORO_IDLE){
    switch (phase) {
      case POMODORO_WORK: // Red
        rgb_matrix_set_color(41, 255, 0, 0);
        rgb_matrix_set_color(42, 255, 0, 0);
        rgb_matrix_set_color(43, 255, 0, 0);
        rgb_matrix_set_color(44, 255, 0, 0);
        break;
      case POMODORO_BREAK: // Green
        rgb_matrix_set_color(41, 0, 255, 0);
        rgb_matrix_set_color(42, 0, 255, 0);
        rgb_matrix_set_color(43, 0, 255, 0);
        rgb_matrix_set_color(44, 0, 255, 0);
        break;
      case POMODORO_LONG_BREAK: // Blue 
        rgb_matrix_set_color(41, 0, 0, 255);
        rgb_matrix_set_color(42, 0, 0, 255);
        rgb_matrix_set_color(44, 0, 0, 255);
        rgb_matrix_set_color(43, 0, 0, 255);
        break;
      case POMODORO_IDLE:
      default:
        break;
    }
  }
  return false;
}  