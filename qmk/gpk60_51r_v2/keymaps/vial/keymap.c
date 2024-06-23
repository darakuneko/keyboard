/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

uint16_t check_rgblight_timer = 0;
uint16_t idle_timer = 0;
int8_t counter = 0;

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSPC,
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, KC_MPLY,
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT,   KC_UP, 
    KC_LCTL,             KC_LALT, KC_LGUI,   MO(1),      KC_SPC,        MO(1),   KC_RGUI,   KC_RALT,                   KC_LEFT, KC_DOWN, KC_RGHT
  ),
  
  [1] = LAYOUT(
    KC_GRV,    KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                 KC_6,      KC_7,       KC_8,      KC_9,      KC_0,      KC_MINS,    KC_EQL, KC_BSLS, 
    KC_F1,     KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,               KC_F7,      KC_F8,      KC_F9,     KC_F10,    KC_F11,  KC_F12,   _______, 
    KC_LSFT,  _______,    _______,    _______,   _______,    _______,         _______,    _______,    _______,    _______,    _______,   KC_LSFT,  _______,
    _______,   _______,    _______,    MO(3),    KC_LSFT,          _______,                   KC_LSFT,   _______,   _______,   _______,   _______, _______
  ),

  [2] = LAYOUT(
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______,_______, 
    _______,  _______,    _______,    _______,   _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, _______, 
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______,
    _______,  _______,    _______,    _______,    _______,            _______,               _______,    _______,    _______,    _______,    _______, _______
  ),

  [3] = LAYOUT(
    _______,  _______,    _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______, _______, 
    _______,  _______,    _______,    _______,   _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______, _______,  _______,
    _______,  _______,     _______,    _______,    _______,    _______,         _______,    _______,    _______,    _______,    _______,    _______,
    _______,  _______,    _______,    _______,    _______,            QK_BOOT,               _______,    _______,    _______,    _______,    _______, _______
 )
};

void keyboard_post_init_user(void) {
  idle_timer = timer_read();
  check_rgblight_timer = timer_read();
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

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && timer_elapsed(idle_timer) > 1000) {
        idle_timer = timer_read();
        counter = 0;
        if (!rgblight_is_enabled()) {
            rgblight_enable_noeeprom();
        }
    }

    return process_record_user(keycode, record);
}

void housekeeping_task_kb(void) {
#ifdef RGBLIGHT_TIMEOUT
  if (timer_elapsed(check_rgblight_timer) > 1000) {
    check_rgblight_timer = timer_read();

    if (rgblight_is_enabled() && timer_elapsed(idle_timer) > 10000) {
      idle_timer = timer_read();
      counter++;
    }

    if (rgblight_is_enabled() && counter > RGBLIGHT_TIMEOUT * 6) {
      counter = 0;
      rgblight_disable_noeeprom();
    }
  }
    
#endif

}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [1] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [2] = { ENCODER_CCW_CW(KC_NO, KC_NO) },
    [3] = { ENCODER_CCW_CW(KC_NO, KC_NO) }
};