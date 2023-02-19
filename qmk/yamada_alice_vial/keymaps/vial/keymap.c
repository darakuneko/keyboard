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
    KC_ESC,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  RGB_MOD,    KC_DEL, KC_F24,
    KC_GRV,  KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,    KC_HOME, 
    KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,    KC_END, 
    KC_LCTL, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,              KC_PGUP,
    KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,               KC_UP, KC_PGDN,
    KC_LGUI, KC_LALT, KC_LCTL,                KC_SPC,         KC_SPC, KC_RCTL, KC_RALT, KC_RCTL,               KC_LEFT, KC_DOWN, KC_RGHT    ),
[1] = LAYOUT(
    _______, RGB_VAI, RGB_VAD, RGB_SAI, RGB_SAD, RGB_HUI, RGB_HUD, RGB_MOD, RGB_RMOD, RGB_TOG, _______, _______, _______, _______, QK_BOOT, KC_F24,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, EE_CLR,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

[2] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F24,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

[3] = LAYOUT(
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F24,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, 
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =  { ENCODER_CCW_CW(C(KC_Z),    C(KC_Y)) },
    [1] =  { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [2] =  { ENCODER_CCW_CW(KC_NO,      KC_NO) },
    [3] =  { ENCODER_CCW_CW(RGB_RMOD,   RGB_MOD) },
};


void encoder_layer_up(void) { 
  if (get_highest_layer(layer_state|default_layer_state) == 3 ) {
    layer_clear();
  } else {
    layer_move(get_highest_layer(layer_state)+1); 
  }
} 

void encoder_layer_down(void) { 
    if (get_highest_layer(layer_state|default_layer_state) == 0 ) {
      layer_move(3);
    } else {
      layer_move(get_highest_layer(layer_state)-1); 
    }
}  

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case KC_F24:
      if (record->event.pressed) {    
        encoder_layer_up();
      }
      return false;
    default:
      return true;
  }
};
 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, NO_LED, },
    { 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, },
    { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, },
    { 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, },
    { 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, },
    { 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83  },
}, {
  // LED Index to Physical Position
  {0, 0},   {16, 0},   {32, 0},   {48, 0},   {64, 0},   {80, 0},   {96, 0},   {112, 0},   {128, 0},   {144, 0},   {160, 0},   {176, 0},  {192, 0},  {208, 0},  {224, 0},
  {0, 13},  {16, 13},  {32, 13},  {48, 13},  {64, 13},  {80, 13},  {96, 13},  {112, 13},  {128, 13},  {144, 13},  {160, 13},  {176, 13}, {192, 13}, {208, 13}, {224, 13},
  {0, 26},  {16, 26},  {32, 26},  {48, 26},  {64, 26},  {80, 26},  {96, 26},  {112, 26},  {128, 26},  {144, 26},  {160, 26},  {176, 26}, {192, 26}, {208, 26}, {224, 26},
  {0, 39},  {16, 39},  {32, 39},  {48, 39},  {64, 39},  {80, 39},  {96, 39},  {112, 39},  {128, 39},  {144, 39},  {160, 39},  {176, 39}, {192, 39},            {224, 39},
  {0, 52},  {16, 52},  {32, 52},  {48, 52},  {64, 52},  {80, 52},  {96, 52},  {112, 52},  {128, 52},  {144, 52},  {160, 52},  {176, 52}, {192, 52}, {208, 52},
  {0, 64},             {32, 64},  {48, 64},  {64, 64},  {80, 64},  {96, 64},  {112, 64},                                      {176, 64}, {192, 64}, {208, 64}, {224, 64},
  }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
		hsv.h = 191; //PURPLE
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (is_layer == 3)  {
      hsv.h = 43; //YELLOW
    } else {
		hsv.h = 128; //CYAN
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};