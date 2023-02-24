/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,       MT(MOD_LSFT, KC_SLSH),
		KC_LGUI, KC_LALT, MO(2),   LT(1, KC_SPC),LT(1, KC_BSPC),MO(2),   KC_RALT, KC_RGUI
	),

	[1] = LAYOUT(
		KC_GRV,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MINS, KC_EQL,  KC_DEL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_BSLS,
		KC_NO,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,   KC_LBRC, KC_RBRC, KC_SCLN, KC_QUOT,
		KC_NO,   MO(3),   KC_NO,   KC_LSFT, KC_LSFT, KC_NO,   MO(3),   KC_NO
	),

	[2] = LAYOUT(
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
		KC_NO,   RGB_VAI, RGB_SAI, RGB_HUI, RGB_SPI, RGB_MOD, RGB_TOG, KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   RGB_VAD, RGB_SAD, RGB_HUD, RGB_SPD, RGB_RMOD,KC_NO,   KC_NO,   KC_NO,   KC_NO,   EEP_RST,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT
	),

	[3] = LAYOUT(
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

};


led_config_t g_led_config = { {
	// Key Matrix to LED Index
	{ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, }, 
	{12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, }, 
	{23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, }, 
	{34, 35, 36, 37, 38, 39, 40, 41 },
}, {
	{0,0}, {20.36,0}, {40.73,0}, {61.09,0}, {81.45,0}, {101.82,0}, {122.18,0}, {142.55,0}, {162.91,0}, {183.27,0}, {203.64,0}, {224,0}, 
	{5.09,21.33}, {25.45,21.33}, {45.82,21.33}, {66.18,21.33}, {86.54,21.33}, {106.91,21.33}, {127.27,21.33}, {147.64,21.33}, {168,21.33}, {188.36,21.33}, {224,21.33}, 
	{15.27,42.67}, {35.63,42.67}, {56,42.67}, {76.36,42.67}, {96.72,42.67}, {117.09,42.67}, {137.45,42.67}, {157.82,42.67}, {178.18,42.67}, {198.54,42.67}, {224,42.67}, 
	{0,64}, {25.45,64}, {45.82,64}, {101.82,64}, {147.63,64}, {168,64}, {193.45,64}, {224,64}
}, {
  	// LED Index to Flag
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	4, 4, 4, 4, 4, 4, 4, 4,
	2, 2, 2, 2, 2, 2, 2, 2
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