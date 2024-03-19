/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "drivers/haptic/drv2605l.h"
#include <stdio.h>
#include "iqs5xx.h"
#include "trackpad_config.h"
#include <math.h>

enum {
  U_Layer_UP = QK_KB_0,
  U_Layer_DOWN,
  U_M_ACL_2x,
  U_M_ACL_4x,
  U_M_ACL_HALF,
  U_M_ACL_QUARTER,
  U_S_ACL_2x,
  U_S_ACL_4x,
  U_S_ACL_8x
};

int end_layer = 4;
int lasted_layer = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	
  [0] = LAYOUT(
    	MT(MOD_LALT,KC_ESC), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RALT,KC_BSLS), 
    	MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_LCTL,KC_QUOT),  
    	KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT, 
		  	                           KC_LGUI, LT(2, KC_DEL),   LT(1, KC_SPC),       LT(1, KC_ENT), LT(2,KC_BSPC), KC_RGUI,
      LGUI(KC_TAB), LGUI(KC_TAB),
      LALT(KC_RGHT),     LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
      LCTL(KC_PPLS), LCTL(KC_PMNS),
      KC_ESC, KC_BTN1, KC_BTN1, KC_BTN1, KC_BTN2                    
  ),

	[1] = LAYOUT(
		KC_GRV,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_LBRC, KC_RBRC,   KC_MINS, KC_EQL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
		KC_LSFT,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_NO,   KC_NO,   KC_NO,   KC_SCLN, KC_QUOT,  KC_LSFT,
    MO(3),    KC_NO,   KC_LSFT,  KC_LSFT,   KC_NO,   MO(3),
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,  
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	[2] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,   KC_F8,  KC_F9,  KC_F10,   KC_F11,  KC_F12,  
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	[3] = LAYOUT(
		RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_SPI,   RGB_MOD,    RGB_TOG,  KC_NO,   KC_NO,   KC_NO,  QK_BOOT,   QK_CLEAR_EEPROM,   KC_NO, 
		RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_SPD,   RGB_RMOD,   KC_NO,    KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	[4] = LAYOUT(
    MT(MOD_LALT,KC_ESC), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RALT,KC_BSLS), 
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_LCTL,KC_QUOT),  
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT, 
		                        U_M_ACL_4x,   U_M_ACL_2x,   KC_BTN1,   KC_BTN2, U_S_ACL_2x,   U_S_ACL_4x,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),
};

void gpk_custom_value_command_kb(uint8_t *data, uint8_t length) {
  uint8_t *command_id = &(data[0]);
  if(*command_id == 119){
    send_trackpad_config(&trackpad_config);
  }
  if(*command_id == 120){
    receive_trackpad_config(data);
  }
}

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
    default:
      return true;
  }
}

void keyboard_post_init_user(void) {
  eeconfig_read_user_datablock(&trackpad_config);
  if(!trackpad_config.init) {
    init_trackpad_config(&trackpad_config);
    drv2605l_pulse(53);
  }
  set_trackpad_config(trackpad_config);
}

void matrix_scan_user(void) {
  int current_layer = get_highest_layer(layer_state|default_layer_state); 
  if(current_layer != trackpad_layer && can_hf_for_layer && lasted_layer != current_layer){
    drv2605l_pulse(hf_waveform_number);
    lasted_layer = current_layer;
  }
}

bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    return false;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int current_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgblight_get_val()};
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
  
  for (uint8_t i = led_min; i <= led_max -1; i++) {
      if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
      }
  }
  return false;
}  