/* Copyright 2020 David Philip Barr <@davidphilipbarr>
 * Copyright 2021 @filterpaper
 * SPDX-License-Identifier: GPL-2.0+
 */

#include QMK_KEYBOARD_H

bool is_hold = false;
  
int layer_num = 3;
uint16_t keycode_t;
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5,
        KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_DOT, KC_QUOT, KC_COMMA, KC_SPC, KC_BSPC,
        KC_ESC, KC_ENT, 
        KC_F24, C(KC_C), C(KC_Z), C(KC_V), C(KC_Y),
        C(KC_PMNS), C(KC_PPLS)
    ),
    [1] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5,
        KC_6, KC_7, KC_8, KC_9, KC_0,
        KC_DOT, KC_QUOT, KC_COMMA, KC_SPC, KC_BSPC,
        KC_BTN2, KC_BTN1, 
        KC_F24, KC_MS_UP, KC_MS_LEFT, KC_MS_DOWN, KC_MS_RIGHT,
        RGB_RMOD, RGB_MOD
    ),
    [2] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, 
        KC_F24,KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO
    ),
    [3] = LAYOUT(
        RGB_VAI,  RGB_SAI,  RGB_HUI, RGB_SPI, RGB_TOG, 
        RGB_VAD,  RGB_SAD,  RGB_HUD, RGB_SPD, QK_BOOT,
        DT_PRNT,  DT_UP,    DT_DOWN, KC_NO,   KC_NO,
        KC_NO, KC_NO, 
        KC_F24,KC_NO, KC_NO, KC_NO, KC_NO,
        RGB_RMOD, RGB_MOD
    )
};


keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 0, .col = 5},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 0, .col = 6},
    .pressed = false
}; 

void matrix_scan_user(void) {
  if(!is_hold){
    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }
  }
}

void encoder_layer_up(void) { 
  if (get_highest_layer(layer_state|default_layer_state) == layer_num ) {
    layer_clear();
  } else {
    layer_move(get_highest_layer(layer_state)+1); 
  }
} 

void encoder_layer_down(void) { 
    if (get_highest_layer(layer_state|default_layer_state) == 0 ) {
      layer_move(layer_num);
    } else {
      layer_move(get_highest_layer(layer_state)-1); 
    }
}  

void encoder_ccw(void) {
  encoder1_ccw.pressed = true;
  encoder1_ccw.time = (timer_read() | 1);
  is_hold ? encoder_layer_down() : action_exec(encoder1_ccw);
}

void encoder_cw(void) {
  encoder1_cw.pressed = true;
  encoder1_cw.time = (timer_read() | 1);
  is_hold ? encoder_layer_up() : action_exec(encoder1_cw);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { clockwise ? encoder_cw() : encoder_ccw(); }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t pressed_time = 0;
  switch (keycode) {  
    case KC_F24:
      if (record->event.pressed) {    
        pressed_time = record->event.time;
        if(!is_hold){
            encoder_layer_up();
        }
        is_hold = false;
      } else {
          if((record->event.time - pressed_time) > TAPPING_TERM) {
            is_hold = true;
          }
      }
      return false;
    default:
      return true;
  }
};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, }, \
    { NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, 0,   1,   2,   3,   4,   5,    },  \
    { NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, 6,   7,   8,   9,   10,  11,   },  \
    { NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, 12,  13,   14,   15,   16,  NO_LED}  \
}, {
  // LED Index to Physical Position
   {0, 0},   {61, 0},   {122, 0},   {183, 0},   {224, 0},
   {0, 21},  {61, 21},  {122, 21},  {183, 21},  {224, 21},
   {0, 42},  {61, 42},  {122, 42},  {183, 42},  {224,42},
   {183, 64},  {224, 64},
  }, {
  // LED Index to Flag
  4, 4, 4, 4, 4,
  4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 
  4, 4,
  2, 2, 2, 2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (current_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (current_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (current_layer == 3)  {
      hsv.h = 0; //PURPLE
    } else if (current_layer == 4)  {
      hsv.h = 170; //BLUE
    } else if (current_layer == 5)  {
      hsv.h = 64; //CHARTREUSE
    } else if (current_layer == 6)  {
      hsv.h = 11; //CORAL
      hsv.s = 176;
    } else if (current_layer == 7)  {
      hsv.h = 36; //GOLD
    } else {
      hsv.h = 213; //MAGENTA
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02) && rgb_matrix_config.mode != 38) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};
