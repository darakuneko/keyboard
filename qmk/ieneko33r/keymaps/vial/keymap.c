/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_LALT, KC_ENT),
		KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
		LT(1, KC_SPC), KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  LT(1, KC_BSPC),
		MT(MOD_LCTL,KC_TAB)
	),
 
	[1] = LAYOUT(
		KC_ESC, KC_GRV,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS, KC_EQL,  XXXXXXX, 
		KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_QUOT,
		KC_LSFT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LBRC, KC_RBRC, KC_SLSH, KC_BSLS, KC_LGUI,
		MO(2)
	),

	[2] = LAYOUT(
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX
	),

	[3] = LAYOUT(
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX
	),

};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int curr_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgb_matrix_get_val()};
  if (curr_layer == 1) {
    hsv.h = 43; //YELLOW
  } else if (curr_layer == 2)  {
    hsv.h = 11; //CORAL
  } else if (curr_layer == 3)  {
    hsv.h = 191; //PURPLE
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);
 
  for (uint8_t i = led_min; i <= led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
  return false;
};
