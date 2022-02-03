/* Copyright 2021 daraku-neko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY o FITNESS FOR A PARTICULAR PURPOSE.  See the
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
    KC_Q,        KC_W,    KC_E,    KC_R,    KC_T,                                   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    KC_A,        KC_S,    KC_D,    KC_F,    KC_G,                                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
    SFT_T(KC_Z), KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC,        KC_RBRC,       KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH),
    KC_TAB,      KC_LGUI, KC_LALT, KC_LCTL, KC_ESC,  LT(1, KC_SPC),  LT(1, KC_ENT), KC_BSPC, KC_LCTL, KC_QUOT, KC_MINS, KC_EQL,
    KC_F24,      C(KC_Z), C(KC_Y) 
  ),

  [1] = LAYOUT(
    S(KC_1), S(KC_2),    S(KC_3),   S(KC_4), S(KC_5),                                S(KC_6), S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0), 
    KC_1,    KC_2,       KC_3,      KC_4,    KC_5,                                   KC_LEFT, KC_DOWN,    KC_UP,      KC_RIGHT,   S(KC_SCLN),
    KC_6,    KC_7,       KC_8,      KC_9,    KC_0,    KC_GRV,         S(KC_GRV),     KC_BSLS, S(KC_BSLS), S(KC_COMM), S(KC_DOT),  S(KC_SLSH),
    KC_HOME, KC_END,     KC_PGUP,   KC_PGDN, MO(6),   LT(2, KC_SPC),  LT(2, KC_ENT), KC_DEL,  MO(6),      S(KC_QUOT), S(KC_MINS), S(KC_EQL),
    KC_F24,  C(KC_PMNS), C(KC_PPLS) 
     ),
  
  [2] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                      _______, _______, _______, _______, _______,
    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                     _______, _______, _______, _______, _______,
    KC_F11,  KC_F12,  _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    KC_F24,  _______, _______ 
     ),

  [3] = LAYOUT(
    KC_Q,        KC_W,    KC_E,    KC_R,    KC_T,                                   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
    KC_A,        KC_S,    KC_D,    KC_F,    KC_G,                                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
    SFT_T(KC_Z), KC_X,    KC_C,    KC_V,    KC_B,    KC_LBRC,        KC_RBRC,       KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH),
    KC_TAB,      KC_LGUI, KC_LALT, KC_LCTL, KC_ESC,  LT(4, KC_SPC),  LT(4, KC_ENT), KC_BSPC, KC_LCTL, KC_QUOT, KC_MINS, KC_EQL,
    KC_F24,      C(KC_Z), C(KC_Y) 
  ),

  [4] = LAYOUT(
    S(KC_1), S(KC_2),    S(KC_3),   S(KC_4), S(KC_5),                                S(KC_6), S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0), 
    KC_1,    KC_2,       KC_3,      KC_4,    KC_5,                                   KC_LEFT, KC_DOWN,    KC_UP,      KC_RIGHT,   S(KC_SCLN),
    KC_6,    KC_7,       KC_8,      KC_9,    KC_0,    KC_GRV,         S(KC_GRV),     KC_BSLS, S(KC_BSLS), S(KC_COMM), S(KC_DOT),  S(KC_SLSH),
    KC_HOME, KC_END,     KC_PGUP,   KC_PGDN, MO(6),   LT(5, KC_SPC),  LT(5, KC_ENT), KC_DEL,  MO(6),      S(KC_QUOT), S(KC_MINS), S(KC_EQL),
    KC_F24,  C(KC_PMNS), C(KC_PPLS) 
  ),

  [5] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                      _______, _______, _______, _______, _______,
    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,                     _______, _______, _______, _______, _______,
    KC_F11,  KC_F12,  _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    KC_F24,  _______, _______ 
  ),

  [6] = LAYOUT(
    RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD, RGB_TOG,                    _______, _______, _______, _______, _______,
    RGB_VAD, RGB_SAD, RGB_HUD, RGB_RMOD, RESET,                     _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, _______,
    KC_F24,  RGB_RMOD, RGB_MOD    
  )
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 1, .col = 11},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 2, .col = 11},
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
        if (get_highest_layer(layer_state|default_layer_state) == 6 ) {
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
  { 0, 4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44,},
  { 1, 5,  9, 13, 17, 21, 25, 29, 33, 37, 41, },
  { 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, },
  { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, }
}, {
  // LED Index to Physical Position
    {0, 0},  {25, 0},  {50, 0},  {75, 0},  {100, 0},  {125, 0},  {150, 0},  {175, 0},  {200, 0},  {224, 0},
    {0, 21}, {25, 21}, {50, 21}, {75, 21}, {100, 21}, {125, 21}, {150, 21}, {175, 21}, {200, 21}, {224, 21},
    {0, 42}, {20, 42}, {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42},
    {0, 63}, {20, 63}, {40, 63}, {60, 63}, {80, 63}, {100, 63}, {120, 63}, {140, 63}, {160, 63}, {180, 63}, {200, 63}, {220, 63}
    }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
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
    } else if (is_layer == 6)  {
      hsv.h = 64; //CHARTREUSE
    } else {
      hsv.h = 0; //RED
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};

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

    oled_write_ln_P(PSTR("GPK60-44UZ1RE"), false);
    oled_write_ln(layer, false);
    render_rgb_status();
}
