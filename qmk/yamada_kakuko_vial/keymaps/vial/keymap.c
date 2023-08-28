/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NUM,  KC_HOME, KC_END,
		KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_PSLS, KC_PAST, KC_PMNS,
		KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_P7,   KC_P8,   KC_P9,  
		KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,  KC_P4,   KC_P5,   KC_P6,  KC_MPLY,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_BSLS, KC_RSFT, KC_P1,   KC_P2,   KC_P3,  KC_MUTE,
		MO(1),   KC_LGUI, KC_LALT, KC_DEL,  LT(1, KC_SPC), LT(1, KC_ENT),  KC_BSPC,   KC_RSFT, KC_RALT, KC_RGUI, MO(2),   KC_P0,   KC_PDOT, KC_PENT,   RGB_TOG
  ),

	[1] = LAYOUT(
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PGUP, KC_PGDN,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_PPLS,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_UP,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_RIGHT,KC_DOWN, KC_LEFT,   KC_NO
	),

	[2] = LAYOUT(
		QK_BOOT, EE_CLR,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO ,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

	[3] = LAYOUT(
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
	),

};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
    [1] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) },
    [2] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) },
    [3] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) }
};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15},
    { 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31},
    { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47},
    { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63},
    { 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    { 80, 81, 82, 83, 84, KC_NO,86,KC_NO, 88, 89, 90, 91, 92, 93, 94},
}, {
  // LED Index to Physical Position
  {0, 0},   {15, 0},   {30, 0},   {45, 0},   {60, 0},   {75, 0},   {90, 0},   {105, 0},   {120, 0},   {135, 0},   {150, 0},   {165, 0},  {180, 0},  {195, 0},  {210, 0},  {224, 0},
  {0, 13},  {15, 13},  {30, 13},  {45, 13},  {60, 13},  {75, 13},  {90, 13},  {105, 13},  {120, 13},  {135, 13},  {150, 13},  {165, 13}, {180, 13}, {195, 13}, {210, 13}, {224, 13},
  {0, 26},  {15, 26},  {30, 26},  {45, 26},  {60, 26},  {75, 26},  {90, 26},  {105, 26},  {120, 26},  {135, 26},  {150, 26},  {165, 26}, {180, 26}, {195, 26}, {210, 26}, {224, 26},
  {0, 39},  {15, 39},  {30, 39},  {45, 39},  {60, 39},  {75, 39},  {90, 39},  {105, 39},  {120, 39},  {135, 39},  {150, 39},  {165, 39}, {180, 39}, {195, 39}, {210, 39}, {224, 39},
  {0, 52},  {15, 52},  {30, 52},  {45, 52},  {60, 52},  {75, 52},  {90, 52},  {105, 52},  {120, 52},  {135, 52},  {150, 52},  {165, 52}, {180, 52}, {195, 52}, {210, 52}, {224, 52},
  {0, 64},  {15, 64},  {30, 64},  {45, 64},      {67.5, 64},           {97.5, 64},        {120, 52},  {135, 52},  {150, 52},  {165, 64}, {180, 64}, {195, 64}, {210, 64}, {224, 64},
  }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 
  } 
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (is_layer == 3)  {
    hsv.h = 43; //YELLOW
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

