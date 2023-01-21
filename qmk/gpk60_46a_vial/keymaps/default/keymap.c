/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(

		KC_ESC,  KC_Q,    KC_P,    KC_RBRC, KC_LBRC,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_SCLN, KC_QUOT, KC_BSLS,
		KC_LSFT, KC_Z,    KC_COMM, KC_DOT,  MT(MOD_LSFT, KC_SCLN),
		KC_LALT, KC_RALT,
		KC_W,    KC_E,    KC_R,    KC_T,
		KC_S,    KC_D,    KC_F,    KC_G,
		KC_X,    KC_C,    KC_V,    KC_B,
		KC_LGUI, MO(2),   LT(1, KC_SPC),
		KC_Y,    KC_U,    KC_I,    KC_O,
		KC_H,    KC_J,    KC_K,    KC_L,
		KC_N,    KC_M,    KC_COMM,
		LT(1, KC_ENT),LT(2, KC_BSPC),KC_RGUI
	),

	[1] = LAYOUT(

		KC_GRV,  KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_1,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_6,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_2,    KC_3,    KC_4,    KC_5,
		KC_7,    KC_8,    KC_9,    KC_0,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO
	),

	[2] = LAYOUT(

		KC_NO,   KC_F1,   KC_F10,  KC_F11,  KC_F12,
		RGB_TOG, RGB_VAI, KC_NO,   KC_NO,   KC_NO,
		KC_NO,   RGB_VAD, KC_NO,   KC_NO,   KC_NO,
		EE_CLR,  QK_BOOT,
		KC_F2,   KC_F3,   KC_F4,   KC_F5,
		RGB_SAI, RGB_HUI, RGB_SPI, KC_NO,
		RGB_SAD, RGB_HUD, RGB_SPD, KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_F6,   KC_F7,   KC_F8,   KC_F9,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO
	),

	[3] = LAYOUT(

		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO
	),

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
