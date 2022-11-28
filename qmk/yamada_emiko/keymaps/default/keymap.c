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
    KC_ESC,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PGUP,   KC_PGDN,    KC_DEL,  KC_PSLS, KC_PMNS,  
    KC_GRV,  KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,               KC_P7,   KC_P8,   KC_P9,
    KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,               KC_P4,   KC_P5,   KC_P6,
    KC_LCTL, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,                         KC_P1,   KC_P2,   KC_P3,
    KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,               KC_UP,                     KC_P0,   KC_PDOT,
    MO(1),   KC_LGUI, KC_LALT,KC_LCTL,                KC_SPC,         KC_RCTL, KC_RALT, KC_RGUI,               KC_LEFT, KC_DOWN, KC_RGHT
    ),

[1] = LAYOUT(
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO, KC_NO,    KC_NLCK, KC_CAPS, KC_SCRL,  
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,           KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,           KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO,                  KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,         KC_NO,                   KC_NO,   KC_NO,
    KC_NO,  MO(4),    KC_NO,   KC_NO,               KC_NO,         KC_NO,   KC_NO,   KC_NO,                      KC_NO, KC_NO, KC_NO
),

[2] = LAYOUT(
    KC_ESC,  KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PGUP,   KC_PGDN,    KC_DEL,  KC_PSLS, KC_PMNS,  
    KC_GRV,  KC_1,    KC_2,   KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,               KC_P7,   KC_P8,   KC_P9,
    KC_TAB,  KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,               KC_P4,   KC_P5,   KC_P6,
    KC_LCTL, KC_A,    KC_S,   KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,                         KC_P1,   KC_P2,   KC_P3,
    KC_LSFT, KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,               KC_UP,                     KC_P0,   KC_PDOT,
    MO(3),   KC_LGUI, KC_LALT,KC_LCTL,                KC_SPC,         KC_RCTL, KC_RALT, KC_RGUI,               KC_LEFT, KC_DOWN, KC_RGHT
    ),

[3] = LAYOUT(
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO, KC_NO,    KC_NLCK, KC_CAPS, KC_SCRL,  
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,           KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,           KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO,                  KC_NO,   KC_NO,   KC_NO,
    KC_NO,  KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,         KC_NO,                   KC_NO,   KC_NO,
    KC_NO,  MO(4),    KC_NO,   KC_NO,               KC_NO,         KC_NO,   KC_NO,   KC_NO,                      KC_NO, KC_NO, KC_NO
),

[4] = LAYOUT(
    RGB_TOG,  RGB_VAI,  RGB_VAD, RGB_SAI, RGB_SAD, RGB_HUI, RGB_HUD, RGB_SPI, RGB_SPD, RGB_MOD,  RGB_RMOD, KC_NO, KC_NO, KC_NO, KC_NO,    KC_NO,   KC_NO,   KC_NO,  
    KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, EE_CLR,         KC_NO,   KC_NO,   KC_NO,
    KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,           KC_NO,   KC_NO,   KC_NO,
    KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO, QK_BOOT,                KC_NO,   KC_NO,   KC_NO,
    KC_NO,    KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,         KC_NO,                   KC_NO,   KC_NO,
    KC_NO,    KC_NO,    KC_NO,   KC_NO,               KC_NO,         KC_NO,   KC_NO,   KC_NO,                      KC_NO, KC_NO, KC_NO
    )
};
 

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,}, 
  {18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34,}, 
  {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,}, 
  {52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, },
  {68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82,}, 
  {83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93}, 

}, {
  // LED Index to Physical Position
  {0,  0},   {13,  0},   {26,  0},   {39,  0},   {52,  0},   {65,  0},   {78,  0},   {91,  0},   {104,  0},   {117,  0},   {130,  0},   {143,  0},   {156,  0},   {169,  0},   {182,  0},           {195,  0},   {208,  0},   {224,  0},
  {0, 13},   {13, 13},   {26, 13},   {39, 13},   {52, 13},   {65, 13},   {78, 13},   {91, 13},   {104, 13},   {117, 13},   {130, 13},   {143, 13},   {156, 13},         {175, 13},                  {195, 13},   {208, 13},   {224, 13},
  {0, 26},   {13, 26},   {26, 26},   {39, 26},   {52, 26},   {65, 26},   {78, 26},   {91, 26},   {104, 26},   {117, 26},   {130, 26},   {143, 26},   {156, 26},         {175, 26},                  {195, 26},   {208, 26},   {224, 26},
  {0, 39},   {13, 39},   {26, 39},   {39, 39},   {52, 39},   {65, 39},   {78, 39},   {91, 39},   {104, 39},   {117, 39},   {130, 39},   {143, 39},                      {175, 39},                  {195, 39},   {208, 39},   {224, 39},
  {0, 52},   {13, 52},   {26, 52},   {39, 52},   {52, 52},   {65, 52},   {78, 52},   {91, 52},   {104, 52},   {117, 52},   {130, 52},   {143, 52},                                {190, 52},                {208, 52},   {224, 52},
  {0, 64},        {19, 64},          {39, 64},               {65, 64},   {78, 64},               {104, 64},                {130, 64},   {143, 64},                             {182, 64},   {190, 64},   {201, 64}
   }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2
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
  } else {
    hsv.h = 191; //PURPLE
  }
  RGB rgb = hsv_to_rgb(hsv);
 
  for (uint8_t i = led_min; i <= led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }

  const led_t led_state = host_keyboard_led_state();
  hsv.h = 0;
  RGB state_rgb = hsv_to_rgb(hsv);
  if (led_state.num_lock) {
    rgb_matrix_set_color(132, state_rgb.r, state_rgb.g, state_rgb.b);
  }
  if (led_state.caps_lock || is_caps_word_on()) {
    rgb_matrix_set_color(134, state_rgb.r, state_rgb.g, state_rgb.b);
  }
  if (led_state.scroll_lock) {
    rgb_matrix_set_color(136, state_rgb.r, state_rgb.g, state_rgb.b);
  }

};