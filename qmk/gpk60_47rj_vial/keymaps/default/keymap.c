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
#include"keymap_japanese.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
 [0] = LAYOUT(
    JP_ZKHK,             KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_AT,   JP_LBRC,
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_COLN, JP_RBRC, KC_ENT,
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    JP_COMM, JP_DOT,  JP_SLSH, JP_BSLS, KC_RSFT,
                         KC_LALT, KC_LGUI, MO(1),            KC_SPC,           MO(1),   JP_HENK, JP_MHEN
  ),
  
 [1] = LAYOUT(  
    KC_ESC,  S(KC_1),   S(KC_2),   S(KC_3),   S(KC_4),   S(KC_5),   S(KC_6),   S(KC_7),    S(KC_8),   S(KC_9),  S(KC_0),  S(JP_MINS), S(JP_CIRC), 
    KC_CAPS, KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LEFT,   KC_DOWN,    KC_UP,     KC_RGHT,  KC_NO,    JP_YEN,     S(JP_YEN),   KC_BSPC,
    KC_NO,   KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_NO,     KC_NO,      KC_NO,     KC_NO,    KC_NO,    KC_NO,      KC_NO,
             KC_MENU,   JP_MHEN,   MO(2),          KC_NO,           MO(2),     JP_HENK,    KC_NO                                                     
  ),

 [2] = LAYOUT(
    KC_NO,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,    KC_F11,  KC_F12,
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,     KC_NO,   KC_NO, QK_BOOT,
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,     KC_NO,   KC_NO,
             KC_NO,     KC_NO,      KC_NO,          KC_NO,            KC_NO,      KC_NO,      KC_NO  
 ),

 [3] = LAYOUT(
    JP_ZKHK,             KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    JP_AT,   JP_LBRC,
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    JP_SCLN, JP_COLN, JP_RBRC, KC_ENT,
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    JP_COMM, JP_DOT,  JP_SLSH, JP_BSLS, KC_RSFT,
                         KC_LALT, KC_LGUI, MO(4),            KC_SPC,           MO(4),   JP_KANA, KC_RALT
  ),
  
 [4] = LAYOUT(  
    KC_ESC,  S(KC_1),   S(KC_2),   S(KC_3),   S(KC_4),   S(KC_5),   S(KC_6),   S(KC_7),    S(KC_8),   S(KC_9),  S(KC_0),  S(JP_MINS), S(JP_CIRC), 
    KC_CAPS, KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LEFT,   KC_DOWN,    KC_UP,     KC_RGHT,  KC_NO,    JP_YEN,     S(JP_YEN),   KC_BSPC,
    KC_NO,   KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_NO,     KC_NO,      KC_NO,     KC_NO,    KC_NO,    KC_NO,      KC_NO,
             KC_MENU,   JP_MHEN,   MO(5),          KC_NO,           MO(5),     JP_HENK,    KC_NO                                                     
  ),

 [5] = LAYOUT(
    KC_NO,  KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,    KC_F11,  KC_F12,
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,      KC_NO,    KC_NO,     KC_NO,   KC_NO, QK_BOOT,
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,   KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,     KC_NO,   KC_NO,
             KC_NO,     KC_NO,      KC_NO,          KC_NO,            KC_NO,      KC_NO,      KC_NO  
 )

};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,  }, 
    { 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,   }, 
    { 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, }, 
    { 40, 41, 42, 43, 44, 45, 46, }, 
}, {
  // LED Index to Physical Position
    {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0},  {224, 0},
    {0, 21}, {17, 21}, {34, 21}, {51, 21}, {68, 21}, {85, 21},  {105, 21}, {119, 21}, {136, 21}, {153, 21}, {170, 21}, {187, 21}, {204, 21}, {224, 21},
    {0, 42}, {20, 42}, {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42}, {224, 42},
                       {40, 63}, {60, 63}, {80, 63}, {100, 63}, {120, 63}, {140, 63}, {160, 63}, 
    }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
  } 
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_val = rgb_matrix_get_val();

    //Lower than the value of RGB_MATRIX_MAXIMUM_BRIGHTNESS, if changed.
    if(current_val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
      rgb_matrix_decrease_val();
      rgb_matrix_increase_val();
    }
    
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, current_val};
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
      hsv.h = 191; //PURPLE
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
};
