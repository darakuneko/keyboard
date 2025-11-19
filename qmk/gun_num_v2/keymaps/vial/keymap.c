/* Copyright 2025 Daraku-Neko
 * SPDX-License-Identifier: GPL-2.0+
 */

#include QMK_KEYBOARD_H

enum {
  U_Layer_UP = QK_KB_0,
};

int layer_num = 7;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5,
        KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_COMMA, KC_DOT, KC_QUOT, KC_SPC, KC_BSPC, KC_ESC, KC_ENT,
        U_Layer_UP, KC_UP, KC_LEFT, KC_DOWN, KC_RIGHT
    ),
    [1] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [2] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [3] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [4] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [5] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [6] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    ),
    [7] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        U_Layer_UP, KC_NO, KC_NO, KC_NO, KC_NO
    )
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [2] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [3] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [4] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [5] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [6] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [7] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    int rgb_matrix_mode = rgb_matrix_get_mode();
    if(rgb_matrix_mode != 1){ return false; }
    int current_layer = get_highest_layer(layer_state|default_layer_state);
    
    HSV hsv = {0, 255, rgb_matrix_get_val()};
    if (current_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (current_layer == 2) {
      hsv.h = 85; //GREEN
    } else if (current_layer == 3) {
      hsv.h = 0; //RED
    } else if (current_layer == 4) {
      hsv.h = 170; //BLUE
    } else if (current_layer == 5) {
      hsv.h = 11; //CORAL
      hsv.s = 176;
    } else if (current_layer == 6) {
      hsv.h = 64; //CHARTREUSE
    } else if (current_layer == 7) {
      hsv.h = 43; //YELLOW
    } else {
      hsv.h = 213; //MAGENTA
    }
    RGB rgb = hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i <= led_max; i++) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return false;
};

void layer_up(void) {
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == layer_num) {
    layer_clear();
  } else {
    layer_move(layer + 1);
  }
}

keypos_t center_push = {.row = 3, .col = 0}; 
bool in_center_push_press(void) {
  return matrix_is_on(0, 0) &&
    matrix_is_on(1, 0) && 
    matrix_is_on(2, 0) && 
    matrix_is_on(3, 0);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if(in_center_push_press() && 
  record->event.key.col == center_push.col &&
  record->event.key.row != center_push.row        
  ){ return false; }
  
  switch (keycode) {
    case U_Layer_UP:
      if (record->event.pressed) {
        layer_up();
      }
      return false;
    default:
      return true;
  }
};
