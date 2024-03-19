/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "iqs5xx.h"
#include "drivers/haptic/drv2605l.h"
#include "trackpad_config.h"
#include <math.h>

int end_layer = 6;
int lasted_layer;

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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_ACL_4x, U_M_ACL_2x, 
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
    KC_BTN1, KC_BTN2, QK_BOOT, EE_CLR, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  )
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
  eeconfig_read_user_datablock(&trackpad_config);
  if(!trackpad_config.init) {
    init_trackpad_config(&trackpad_config);
    drv2605l_pulse(53);
  }
  set_trackpad_config(trackpad_config);
}

void matrix_scan_user(void) {
  center_push_press(center_push, &center_push_holding, in_center_push_press());
  int current_layer = get_highest_layer(layer_state|default_layer_state); 
  
  //use can_rackpad_layer
  //if(current_layer != trackpad_layer && can_hf_for_layer && lasted_layer != current_layer){
  if(can_hf_for_layer && lasted_layer != current_layer){
    drv2605l_pulse(hf_waveform_number);
    lasted_layer = current_layer;
  }
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
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
    HSV hsv = {0, 255, rgblight_get_val()};
    if (current_layer == 1) {
      hsv.h = 213; //MAGENTA
    } else if (current_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (current_layer == 3)  {
      hsv.h = 170; //BLUE
    } else if (current_layer == 4)  {
      hsv.h = 11; //CORAL
      hsv.s = 176;
    } else if (current_layer == 5)  {
      hsv.h = 43; //YELLOW
    } else if (current_layer == 6)  {
      hsv.h = 255; //RED
    } else {
      hsv.h = 128; //CYAN
    }
    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
};