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
#include "iqs5xx.h"

typedef union {
  uint32_t raw;
  struct {
    bool     tap : 1;
  };
} user_config_t;
user_config_t user_config;

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    C(KC_Z),   KC_1,    KC_2,     KC_3,    KC_4,          KC_5, 
    KC_F24,    KC_6,    KC_7,     KC_8,    KC_9,          KC_0,
    C(KC_Y),   KC_BSPC, KC_QUOT,  KC_SPC,  SFT_T(KC_ENT), MO(1),
    KC_MS_BTN1, KC_MS_BTN2,
    LGUI(KC_TAB), LGUI(KC_TAB),
    KC_WFWD,     LCTL(LGUI(KC_RIGHT)), KC_WBAK, LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS), LCTL(KC_PMNS)
  ),
 
  [1] = LAYOUT(
    C(KC_Z),   KC_1,    KC_2,     KC_3,    KC_4,          KC_5, 
    KC_F24,    KC_6,    KC_7,     KC_8,    KC_9,          KC_0,
    C(KC_Y),   KC_BSPC, KC_QUOT,  KC_SPC,  SFT_T(KC_ENT), MO(2),
    KC_MS_BTN1, KC_MS_BTN2,     
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS
  ), 

  [2] = LAYOUT(
    C(KC_Z),   KC_1,    KC_2,     KC_3,    KC_4,          KC_5, 
    KC_F24,    KC_6,    KC_7,     KC_8,    KC_9,          KC_0,
    C(KC_Y),   KC_BSPC, KC_QUOT,  KC_SPC,  SFT_T(KC_ENT), MO(3),
    KC_MS_BTN1, KC_MS_BTN2,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS  
  ),
  
  [3] = LAYOUT(
    RGB_RMOD, RGB_VAI,    RGB_SAI,    RGB_HUI,    RGB_SPI,    RGB_TOG,  
    KC_F24,   RGB_VAD,    RGB_SAD,    RGB_HUD,    RGB_SPD,    _______,
    RGB_MOD,  DT_PRNT,    DT_UP,      DT_DOWN,    KC_F23,    QK_BOOT,
    KC_MS_BTN1, KC_MS_BTN2,
    LCTL(KC_DOWN),  LCTL(KC_UP), 
    LGUI(KC_RBRC),  LCTL(KC_RGHT),   LGUI(KC_LBRC), LCTL(KC_LEFT), 
    LGUI(KC_EQL),   LGUI(KC_MINS)
  )
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 2},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 3},
    .pressed = false
}; 

void matrix_scan_user(void) {
    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }
}

bool is_hold = false;

void encoder_layer_up(void) { 
  if (get_highest_layer(layer_state|default_layer_state) == 3 ) {
    layer_clear();
  } else {
    layer_move(get_highest_layer(layer_state)+1); 
  }
} 

void encoder_layer_down(void) { 
    if (get_highest_layer(layer_state|default_layer_state) == 0 ) {
      layer_move(3);
    } else {
      layer_move(get_highest_layer(layer_state)-1); 
    }
}  

void encoder_ccw(void) {
  encoder1_ccw.pressed = true;
  encoder1_ccw.time = (timer_read() | 1);
  is_hold ? encoder_layer_down() : action_exec(encoder1_ccw);
}

void encoder_cw(void) {
  encoder1_cw.pressed = true;
  encoder1_cw.time = (timer_read() | 1);
  is_hold ? encoder_layer_up() : action_exec(encoder1_cw);
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { clockwise ? encoder_cw() : encoder_ccw(); }
    return true;
}

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  tap_mode = user_config.tap;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t pressed_time = 0;
  switch (keycode) {  
      case KC_BTN1 ... KC_BTN5:
        if (record->event.pressed) {
          pointing_device_set_button(1 << (keycode - KC_BTN1));
        } else {
          pointing_device_clear_button(1 << (keycode - KC_BTN1));
        }
      return false;
      break;
    case KC_F24:
      if (record->event.pressed) {    
        pressed_time = record->event.time;
        if(!is_hold){
            encoder_layer_up();
        }
        is_hold = false;
      } else {
          if((record->event.time - pressed_time) > TAPPING_TERM) {
            is_hold = true;
          }
      }
      return false;
    case KC_F23:
      if (record->event.pressed) {    
        user_config.tap = !user_config.tap;  
        eeconfig_update_user(user_config.raw); 
        tap_mode = user_config.tap;
      } 
      return false;  
    default:
      return true;
  }
};

void eeconfig_init_user(void) {
  rgb_matrix_enable();
  rgb_matrix_sethsv(213, 255, 255);
  rgb_matrix_mode(1);
}

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  {  0, 1, 2, 3 },
  {  4, 5, 6, 7 },
  {  8, 9, 10, 11 },
  {  12, 13, 14, NO_LED },
  {  NO_LED, NO_LED, NO_LED, NO_LED }
}, {
  // LED Index to Physical Position
    {0, 0}, {56, 0}, {112, 0}, {168, 0}, {224, 0},
    {0, 32}, {56, 32}, {112, 32}, {168, 32}, {224, 32},
    {0, 64}, {56, 64}, {112, 64}, {168, 64}, {224, 64}
  }, { 
  // LED Index to Flag
  4, 4, 4, 4, 4,
  4, 4, 4, 4, 4,
  4, 4, 4, 4, 4,
  2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (current_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (current_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (current_layer == 3)  {
      hsv.h = 255; //PURPLE
    } else {
      hsv.h = 213; //MAGENTA
    }
    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};

