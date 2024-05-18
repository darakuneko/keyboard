/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

enum {
  IKIRI = QK_KB_0
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,  KC_MPLY, KC_MUTE, RGB_TOG, KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_DEL,
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,                             KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_BSLS, 
    KC_LCTL, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,                             KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B, KC_LBRC,                    KC_RBRC, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                                                         IKIRI,  KC_F1,   KC_F3,
                 KC_LGUI, KC_LCTL, KC_LALT,   LT(1, KC_SPC),                    LT(1, KC_ENT),     KC_BSPC, KC_RCTL, KC_RGUI,
    KC_F2, KC_F4,  KC_F6,    KC_F8,
    KC_F5, KC_F7,  KC_F9,    KC_F11, 
    KC_F10, KC_F12
    ),

[1] = LAYOUT(
    KC_GRV, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,                  KC_LEFT,   KC_DOWN,   KC_UP,      KC_RGHT,     XXXXXXX,    XXXXXXX,
    KC_LSFT, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, KC_MINS, KC_EQL, XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    KC_LSFT,
    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,  XXXXXXX, KC_LSFT,                   KC_LSFT,   MO(2),    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX
    ),

[2] = LAYOUT(
    QK_BOOT, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,      XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
     XXXXXXX,    XXXXXXX,    XXXXXXX,
                               XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX, 
     XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX
    ),

[3] = LAYOUT(
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,  XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,      XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,
                               XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX
    ),    
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case IKIRI:
      if (record->event.pressed) {
        rgb_matrix_mode(50);
      } 
      return false;      
    default:
      return true;
  }
};
 
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
  [0] = { ENCODER_CCW_CW(KC_MPRV, KC_MNXT),  ENCODER_CCW_CW(KC_VOLD, KC_VOLU),  ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
  [1] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) },
  [2] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) },
  [3] = { ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO),  ENCODER_CCW_CW(KC_NO, KC_NO) }
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  if(rgb_matrix_get_mode() != 50) {
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
      } else if (HAS_FLAGS(g_led_config.flags[i], 0x01)) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
      }
    }
  }
	return false;
};
