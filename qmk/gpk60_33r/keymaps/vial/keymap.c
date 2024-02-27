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

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_Q,                  KC_W,          KC_E,    KC_R,    KC_T,   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,           KC_ENT, \
    KC_A,          KC_S,    KC_D,    KC_F,   KC_G,    KC_H,    KC_J,    KC_K,    KC_L,           KC_SCLN, \
    MT(MOD_LSFT,KC_SPC),   LT(1, KC_Z),   KC_X,    KC_C,    KC_V,   KC_B,    KC_N,    KC_M,    KC_COMM, LT(1, KC_DOT),  MT(MOD_RSFT,KC_BSPC) ,MT(MOD_LCTL,KC_TAB) 
  ),

  [1] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,     KC_8,    KC_9,    KC_0,    MT(MOD_LSFT,KC_ESC), \
    KC_GRV,  KC_BSLS, KC_MINS, KC_EQL,  KC_QUOT, KC_LEFT,  KC_DOWN, KC_UP,   KC_RGHT, KC_LGUI, \
    KC_NO, KC_NO, KC_SLSH, KC_LBRC, KC_RBRC, KC_NO, KC_NO,  KC_NO, KC_NO, KC_NO, MO(2), KC_LSFT
  ),

  [2] = LAYOUT(
    RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD, RGB_TOG, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_NO, \
    RGB_VAD, RGB_SAD, RGB_HUD, RGB_RMOD,KC_NO, KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10, KC_NO, \
    KC_NO,  KC_NO,    KC_NO, KC_NO, QK_BOOT,   KC_F11,  KC_F12,  KC_NO, KC_NO, KC_NO, KC_NO
  )
};

void keyboard_post_init_user(void) {
  rgblight_enable_noeeprom();
  rgblight_sethsv_noeeprom(128, 255, rgblight_get_val());
  rgblight_mode_noeeprom(1);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
      case 1: //PURPLE
          rgblight_sethsv_noeeprom(191, 255, rgblight_get_val());
          break;
      case 2: //GREEN
          rgblight_sethsv_noeeprom(85, 255, rgblight_get_val());
          break;
      case 3: //YELLOW
          rgblight_sethsv_noeeprom(43, 255, rgblight_get_val());
          break;   
      default: //CYAN
          rgblight_sethsv_noeeprom(128, 255, rgblight_get_val());
        break;
    } 
  return state;
}