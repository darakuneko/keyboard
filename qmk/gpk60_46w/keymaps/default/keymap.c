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
enum layer_names {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QWERTY] = LAYOUT(
    KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS, \
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                            KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_LBRC,       KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,  \
                                           KC_HENK, MO(2),   MO(1),  KC_SPC,        KC_ENT,   KC_BSPC, MO(1),    MO(2)                      \
  ),

  [_LOWER] = LAYOUT(
    S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),                              S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), KC_TRNS, \
    KC_LGUI,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                              KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_MINS, KC_EQL, \
    KC_F1,     KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    S(KC_MINS),      S(KC_EQL), KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
                                 KC_MHEN, KC_LALT, MO(3),    KC_TRNS,         KC_TRNS,    KC_DEL,  MO(3),   KC_TRNS                     \
  ),

  [_RAISE] = LAYOUT(
    KC_GRV,   KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                                    KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       _______, \
    _______,  _______,    _______,    _______,    _______,    _______,                                 _______,    _______,    _______,    _______,    _______,    _______, \
    _______,  _______,    _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______,    _______, \
                                      _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______  \
  ),

  [_ADJUST] = LAYOUT(
    RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_MOD,    _______,    RGB_TOG,                                 _______,    _______,    _______,    _______,    _______, _______, \
    RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_RMOD,   _______,    _______,                                 _______,    _______,    _______,    _______,    _______, _______, \
    _______,  _______,    _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, \
                                      _______,    _______,    _______,    _______,         RESET,      _______,    _______,    _______                           \

 )
};

void eeconfig_init_user(void) {
  rgblight_enable();
  rgblight_sethsv_magenta();
  rgblight_mode(1);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
      case _RAISE:
         rgblight_sethsv_noeeprom_cyan();
         rgblight_mode_noeeprom(1);
         break;
      case _LOWER:
         rgblight_sethsv_noeeprom_green();
          rgblight_mode_noeeprom(1);
          break;
      case _ADJUST:
      default:
         rgblight_reload_from_eeprom();
          break;
    }
  return state;
}
