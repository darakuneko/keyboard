/* Copyright 2022 hanachi-ap
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
    _BASE,
    _L1,
    _L2,
    _L3,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT( 
	KC_ESC,	   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                     KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,     KC_LBRC,
	KC_TAB,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,  KC_QUOT,
	KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,    KC_B,  KC_DEL,   KC_LGUI,KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH,  KC_SLSH,
    KC_LALT,                   KC_LEFT,KC_RIGHT,KC_SPC,KC_LCTL,  KC_BSPC,KC_ENT, KC_DOWN, KC_UP,                    KC_ENT,
    KC_A, KC_B
    ),


    [_L1] = LAYOUT(
	KC_ESC,	   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                     KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,     KC_LBRC,
	KC_TAB,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,  KC_QUOT,
	KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_DEL,    KC_LGUI,KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_SLSH,
    KC_LGUI,                   KC_LEFT,KC_RIGHT,KC_SPC,KC_LCTL,  KC_BSPC,KC_ENT, KC_DOWN, KC_UP,                      KC_ENT,
    KC_A, KC_B
    ),


    [_L2] = LAYOUT(
	KC_ESC,	   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                     KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,     KC_LBRC,
	KC_TAB,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,  KC_QUOT,
	KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_DEL,    KC_LGUI,KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_SLSH,
    KC_LGUI,                   KC_LEFT,KC_RIGHT,KC_SPC,KC_LCTL,  KC_BSPC,KC_ENT, KC_DOWN, KC_UP,                      KC_ENT,
    KC_A, KC_B
    ),


    [_L3] = LAYOUT(
	KC_ESC,	   KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                     KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,     KC_LBRC,
	KC_TAB,    KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                     KC_H,   KC_J,   KC_K,   KC_L,   KC_SCLN,  KC_QUOT,
	KC_LSFT,   KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,  KC_DEL,    KC_LGUI,KC_N,   KC_M,   KC_COMM,KC_DOT, KC_SLSH, KC_SLSH,
    KC_LGUI,                   KC_LEFT,KC_RIGHT,KC_SPC,KC_LCTL,  KC_BSPC,KC_ENT, KC_DOWN, KC_UP,                      KC_ENT,
    KC_A, KC_B
    )    
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE] = { ENCODER_CCW_CW(KC_C, KC_D), ENCODER_CCW_CW(KC_E, KC_F), ENCODER_CCW_CW(KC_G, KC_H), ENCODER_CCW_CW(KC_I, KC_J) },
    [_L1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_MPRV, KC_MNXT), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [_L3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

void keyboard_post_init_user(void) {
  if(rgblight_get_mode() == 1){
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(128, 255, rgblight_get_val());
    rgblight_mode_noeeprom(1);
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
  if(rgblight_get_mode() == 1){
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
  }

  return state;
}
