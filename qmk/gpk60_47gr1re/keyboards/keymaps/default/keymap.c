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
    KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, \
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_LGUI,                      KC_LALT, MO(2),   MO(1),   KC_SPC,       KC_BSPC, MO(1),   MO(2),   KC_F24,  C(KC_Z), C(KC_Y) \
  ),

  [1] = LAYOUT(
    S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),     S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), S(KC_MINS), S(KC_EQL), S(KC_BSLS),
    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_TRNS,  \
    KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, \
    KC_LGUI,            KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_F24,  _______, _______ \
  ),
  
  [2] = LAYOUT(
    KC_GRV,   KC_1,       KC_2,       KC_3,       KC_4,       KC_5,             KC_6,      KC_7,       KC_8,       KC_9,       KC_0,       KC_MINS, KC_EQL, \
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, \
    _______,                          _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    KC_F24, _______, _______   \
  ),

  [3] = LAYOUT(
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, \
    _______,                          _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    KC_F24, _______, _______\
  ),

  [4] = LAYOUT(
    RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_MOD,    _______,    RGB_TOG,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
    RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_RMOD,   _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, \
    _______,                          _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    KC_F24,  RGB_RMOD, RGB_MOD \
  )
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 1},
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
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    }

    return true;
}


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_F24:
      if (record->event.pressed) {
       if (get_highest_layer(layer_state) == 0) {
         layer_invert(1); 
       } else if (get_highest_layer(layer_state) == 1) {
         layer_invert(2); 
       } else if (get_highest_layer(layer_state) == 2) {
         layer_invert(3); 
       } else if (get_highest_layer(layer_state) == 3) {
         layer_invert(4); 
       } else {
         layer_clear();
       }
      } 
      return false;
    default:
      return true;
  }
};
 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0, 4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44,},
  { 1, 5,  9, 13, 17, 21, 25, 29, 33, 37, 41, 45,},
  { 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46,},
  { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47 }
}, {
  // LED Index to Physical Position
   {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0},
    {0, 21}, {20, 21}, {40, 21}, {60, 21}, {80, 21}, {100, 21}, {120, 21}, {140, 21}, {160, 21}, {180, 21}, {200, 21}, {220, 21},
    {0, 42}, {20, 42}, {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42},
    {0, 63}, {20, 63}, {40, 63}, {60, 63}, {80, 63}, {100, 63}, {120, 63}, {140, 63}, {160, 63}, {180, 63}, {200, 63}
    }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {    
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
        switch(get_highest_layer(layer_state|default_layer_state)) {
            case 1:
                rgb_matrix_set_color(i, RGB_CYAN);
                break;
            case 2:
                rgb_matrix_set_color(i, RGB_GREEN);
                break;
            case 3:
                rgb_matrix_set_color(i, RGB_YELLOW);
                break; 
            case 4:
                rgb_matrix_set_color(i, RGB_WHITE);
                break;
            default:
                rgb_matrix_set_color(i, RGB_MAGENTA);
                break;
        }
      }
    }
};  

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

void render_rgb_status(void) {

    switch(rgb_matrix_config.mode) {
            case 0:
                oled_write_ln_P(PSTR("RGB: NONE"), false);
                break;
            case 1:
                oled_write_ln_P(PSTR("RGB: SOLID_COLOR"), false);
                break;
            case 2:
                oled_write_ln_P(PSTR("RGB: BREATHING"), false);
                break;
            case 3:
                oled_write_ln_P(PSTR("RGB: CYCLE_ALL"), false);
                break;
            case 4:
                oled_write_ln_P(PSTR("RGB: RAINBOW_BEACON"), false);
                  break;                  
            case 5:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_SMPL"), false);
                  break;
            case 6:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT"), false);
                  break;
            case 7:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_WIDE"), false);
                  break;
            case 8:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_CROSS"), false);
                  break;
            case 9:
                  oled_write_ln_P(PSTR("RGB: SPLASH"), false);
                  break;      
            default:
                oled_write_ln_P(PSTR("RGB: OTHER"), false);
                break;
    }
}

void oled_task_user(void) {
    char layer[12];    

    snprintf(layer, sizeof(layer), "Layer: %d", get_highest_layer(layer_state));

    oled_write_ln_P(PSTR("GPK60-47GR1RE"), false);
    oled_write_ln(layer, false);
    render_rgb_status();
}
