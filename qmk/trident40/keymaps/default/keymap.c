/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  MT(MOD_LSFT, KC_SCLN),
		KC_LGUI, KC_LALT, LT(1, KC_SPC),LT(1, KC_BSPC),MO(2),   KC_RALT
	),

	[1] = LAYOUT(
		KC_GRV,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MINS, KC_EQL,  KC_DEL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_NO,   KC_DOWN, KC_UP,   KC_RGHT, KC_NO,
		KC_NO,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,   KC_NO,   KC_LBRC, KC_RBRC, KC_BSLS,
		KC_NO,   MO(3),   KC_LSFT, KC_LSFT, KC_NO,   MO(3)
	),

	[2] = LAYOUT(
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
		KC_NO,   RGB_VAI, RGB_SAI, RGB_HUI, RGB_SPI, RGB_MOD, RGB_TOG, KC_NO,   KC_NO,   KC_NO,   QK_BOOT,
		KC_NO,   RGB_VAD, RGB_SAD, RGB_HUD, RGB_SPD, RGB_RMOD,KC_NO,   KC_NO,   KC_NO,   KC_NO,   EEP_RST,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

	[3] = LAYOUT(
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

};
