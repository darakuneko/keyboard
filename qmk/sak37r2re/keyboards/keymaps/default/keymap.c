/* Copyright 2021 daraku-neko
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
#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
                         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_ENT,     \
    MT(MOD_LCTL, KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH), \
                              LT(2, KC_GRV),   LT(1, KC_SPC),               LT(1, KC_BSPC), LT(2, KC_ESC),    \
    KC_F24,  C(KC_Z), C(KC_Y),  KC_F24,  C(KC_PMNS), C(KC_PPLS)    \
  ),

  [1] = LAYOUT(
             S(KC_1), S(KC_2),    S(KC_3),   S(KC_4), S(KC_5),   S(KC_6), S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0), KC_ENT, 
    KC_LALT, KC_1,    KC_2,       KC_3,      KC_4,    KC_5,      KC_LEFT, KC_DOWN,    KC_UP,      KC_RIGHT,   S(KC_SCLN),
    KC_LGUI, KC_6,    KC_7,       KC_8,      KC_9,    KC_0,      KC_BSLS, S(KC_BSLS), S(KC_COMM), S(KC_DOT),  S(KC_SLSH),
                                             S(KC_GRV),   _______,     KC_DEL, _______,    \
    KC_F24,  C(KC_Z), C(KC_Y),     KC_F24,  RGB_MOD, RGB_RMOD    \
  ),

  [2] = LAYOUT(
             KC_QUOT, S(KC_QUOT),  KC_MINS, S(KC_MINS), KC_EQL,   S(KC_EQL),  KC_LBRC, S(KC_LBRC), KC_RBRC,  S(KC_RBRC),   _______, 
    RESET,   RGB_VAI, RGB_SAI,     RGB_HUI, RGB_MOD,    RGB_TOG,  RGB_VAD,    RGB_SAD, RGB_HUD,    RGB_RMOD, _______,      
    _______, _______,  _______,  _______,  _______,  _______,     _______,  _______,  _______,  _______,  _______,
                                           _______,  _______,     _______,  _______,    \
    KC_F24,  C(KC_Z), C(KC_Y),     KC_F24,  RGB_MOD, RGB_RMOD    \
  ),

  [3] = LAYOUT(
                         KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_ENT,     \
    MT(MOD_LCTL, KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH), \
                              LT(5, KC_GRV),   LT(4, KC_SPC),               LT(5, KC_BSPC), LT(4, KC_ESC),    \
    KC_F24,  C(KC_Z), C(KC_Y),  KC_F24,  C(KC_PMNS), C(KC_PPLS)    \
  ),

  [4] = LAYOUT(
             S(KC_1), S(KC_2),    S(KC_3),   S(KC_4), S(KC_5),   S(KC_6), S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0), KC_ENT, 
    KC_LALT, KC_1,    KC_2,       KC_3,      KC_4,    KC_5,      KC_LEFT, KC_DOWN,    KC_UP,      KC_RIGHT,   S(KC_SCLN),
    KC_LGUI, KC_6,    KC_7,       KC_8,      KC_9,    KC_0,      KC_BSLS, S(KC_BSLS), S(KC_COMM), S(KC_DOT),  S(KC_SLSH),
                                             S(KC_GRV),   _______,     KC_DEL, _______,    \
    KC_F24,  C(KC_Z), C(KC_Y),     KC_F24,  RGB_MOD, RGB_RMOD    \
  ),

  [5] = LAYOUT(
             KC_QUOT, S(KC_QUOT),  KC_MINS, S(KC_MINS), KC_EQL,   S(KC_EQL),  KC_LBRC, S(KC_LBRC), KC_RBRC,  S(KC_RBRC),   _______, 
    RESET,   RGB_VAI, RGB_SAI,     RGB_HUI, RGB_MOD,    RGB_TOG,  RGB_VAD,    RGB_SAD, RGB_HUD,    RGB_RMOD, _______,    
    _______, _______,  _______,  _______,  _______,  _______,     _______,  _______,  _______,  _______,  _______,
                                           _______,  _______,     _______,  _______,    \
    KC_F24,  C(KC_Z), C(KC_Y),     KC_F24,  RGB_MOD, RGB_RMOD    \
  ),
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 8, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 8, .col = 1},
    .pressed = false
};

keyevent_t encoder2_ccw = {
    .key = (keypos_t){.row = 8, .col = 2},
    .pressed = false
};

keyevent_t encoder2_cw = {
    .key = (keypos_t){.row = 8, .col = 3},
    .pressed = false
};

void matrix_scan_user(void) {
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

    if (IS_PRESSED(encoder2_ccw)) {
        encoder2_ccw.pressed = false;
        encoder2_ccw.time = (timer_read() | 1);
        action_exec(encoder2_ccw);
    }

    if (IS_PRESSED(encoder2_cw)) {
        encoder2_cw.pressed = false;
        encoder2_cw.time = (timer_read() | 1);
        action_exec(encoder2_cw);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    } else if (index == 1) {
        if (clockwise) {
            encoder2_cw.pressed = true; 
            encoder2_cw.time = (timer_read() | 1);
            action_exec(encoder2_cw);
        } else {
            encoder2_ccw.pressed = true;
            encoder2_ccw.time = (timer_read() | 1);
            action_exec(encoder2_ccw);
        }
    }

    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_F24:
      if (record->event.pressed) {
        if (get_highest_layer(layer_state|default_layer_state) == 5 ) {
          layer_clear();
        } else {
          layer_invert(get_highest_layer(layer_state)+1); 
        }
      } 
      return false;
    default:
      return true;
  }
};
 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0, 8,  16, 24, 32, },
  { 1, 9,  17, 25, 33, },
  { 2, 10, 18, 26, 34, },
  { 3, 11, 19, 27, 35, },
  { 4, 12, 20, 28, 36, },
  { 5, 13, 21, 29, 37, },
  { 6, 14, 22, 30, 38, },
  { 7, 15, 23, 31,},
}, {
  // LED Index to Physical Position
   {0, 0},  {22.5, 0},  {45, 0},  {67.5, 0},  {90, 0},  {112.5, 0},  {135, 0},  {157.5, 0},  {180, 0},  {202.5, 0},  {224, 0}, 
   {0, 21},  {22.5, 21},  {45, 21},  {67.5, 21},  {90, 21},  {112.5, 21},  {135, 21},  {157.5, 21},  {180, 21},  {202.5, 21},  {224, 21},
   {0, 21},  {22.5, 21},  {45, 21},  {67.5, 21},  {90, 21},  {112.5, 21},  {135, 21},  {157.5, 21},  {180, 21},  {202.5, 21},  {224, 21}, 
                                                  {90, 63},  {112.5, 63},  {135, 63},  {157.5, 63},  
}, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  } 
};


void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (is_layer == 3)  {
      hsv.h = 43; //YELLOW
    } else if (is_layer == 4)  {
      hsv.h = 11; //CORAL
    } else if (is_layer == 5)  {
      hsv.h = 0; //RED
    } else {
      hsv.h = 191; //PURPLE
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};