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
#include <stdio.h>
#include <string.h>
#include "code_to_name.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, \
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_LGUI,                      KC_LALT, MO(2),   MO(1),         KC_SPC,   LT(1,KC_BSPC), LT(2, KC_MINS),  KC_EQL,   KC_F24,  C(KC_Z), C(KC_Y) \
  ),

  [1] = LAYOUT(
    S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),         S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), S(KC_LBRC), S(KC_RBRC), S(KC_BSLS),
    KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, S(KC_SCLN), S(KC_QUOT), KC_NO,  \
    KC_NO,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,            KC_NO,   KC_NO, KC_NO,   KC_NO,  S(KC_SLSH),  S(KC_RSFT), \
    KC_NO,                   KC_NO, KC_NO, KC_NO,        KC_NO,     KC_DEL, S(KC_MINS), S(KC_EQL), KC_F24, C(KC_PMNS), C(KC_PPLS) \
  ),
  
  [2] = LAYOUT(
    KC_NO,    KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,  \
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, KC_NO,  \
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, \
    KC_NO,                          KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_F24,     KC_NO, KC_NO   \
  ),

  [3] = LAYOUT(
    KC_ESC,              KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,           KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, \
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,           KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT, \
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,           KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, \
    KC_LGUI,                      KC_LALT, MO(5),   MO(4),         KC_SPC,   LT(4,KC_BSPC), LT(5, KC_MINS),  KC_EQL,   KC_F24,  C(KC_Z), C(KC_Y) \
  ),

  [4] = LAYOUT(
    S(KC_GRV), S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),         S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0), S(KC_LBRC), S(KC_RBRC), S(KC_BSLS),
    KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,            KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, S(KC_SCLN), S(KC_QUOT), KC_NO,  \
    KC_NO,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,            KC_NO,   KC_NO, KC_NO,   KC_NO,  S(KC_SLSH),  S(KC_RSFT), \
    KC_NO,                   KC_NO, KC_NO, KC_NO,        KC_NO,     KC_DEL, S(KC_MINS), S(KC_EQL), KC_F24, C(KC_PMNS), C(KC_PPLS) \
  ),
  
  [5] = LAYOUT(
    KC_NO,    KC_F1,  KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,       KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,  \
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, KC_NO,  \
    KC_NO,  KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, \
    KC_NO,                          KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_F24,     KC_NO, KC_NO   \
  ),

  [6] = LAYOUT(
    RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_SPI,    RGB_MOD,    RGB_TOG,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, KC_NO, KC_NO,\
    RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_SPD,   RGB_RMOD,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, RESET, \
    DT_PRNT,  DT_UP,    DT_DOWN,    KC_NO,    KC_NO,    KC_NO,         KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, \
    KC_NO,                          KC_NO,    KC_NO,    KC_NO,         RESET,    KC_NO,    KC_NO,    KC_NO,    KC_F24,     RGB_RMOD, RGB_MOD \
  )
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 1},
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
  if (get_highest_layer(layer_state|default_layer_state) == 6 ) {
    layer_clear();
  } else {
    layer_move(get_highest_layer(layer_state)+1); 
  }
} 

void encoder_layer_down(void) { 
    if (get_highest_layer(layer_state|default_layer_state) == 0 ) {
      layer_move(6);
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  static uint16_t pressed_time = 0;
  if (record->event.pressed) {
    set_code_to_name(keycode);
  }
  switch (keycode) {  
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
    default:
      return true;
  }
};

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0, 4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44,},
  { 1, 5,  9, 13, 17, 21, 25, 29, 33, 37, 41, 45,},
  { 2, 6, 10, 14, 18, 22, 26, 30, 34, 38, 42, 46,},
  { 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47 }
}, {
  // LED Index to Physical Position
   {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0},
    {0, 21}, {20, 21}, {40, 21}, {60, 21}, {80, 21}, {100, 21}, {120, 21}, {140, 21}, {160, 21}, {180, 21}, {200, 21}, {220, 21},
    {0, 42}, {20, 42}, {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42},
    {0, 63}, {20, 63}, {40, 63}, {60, 63}, {80, 63}, {100, 63}, {120, 63}, {140, 63}, {160, 63}, {180, 63}, {200, 63}
    }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2
  } 
};


void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (is_layer == 3)  {
      hsv.h = 43; //YELLOW
    } else if (is_layer == 4)  {
      hsv.h = 11; //CORAL
    } else if (is_layer == 5)  {
      hsv.h = 0; //RED
    } else if (is_layer == 6)  {
      hsv.h = 64; //CHARTREUSE
    } else {
      hsv.h = 191; //PURPLE
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

void render_rgb_status(void) {

    switch(rgb_matrix_config.mode) {
            case 0:
                oled_write_ln_P(PSTR("RGB: NONE"), false);
                break;
            case 1:
                oled_write_ln_P(PSTR("RGB: SOLID_COLOR"), false);
                break;        
            case 2:
                oled_write_ln_P(PSTR("RGB: GRADIENT_UPDN"), false);
                break;      
            case 3:
                oled_write_ln_P(PSTR("RGB: GRADIENT_LR"), false);
                break;    
            case 4:
                oled_write_ln_P(PSTR("RGB: CYCLE_ALL"), false);
                break;     
            case 5:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_SMPL"), false);
                  break;
            case 6:
                  oled_write_ln_P(PSTR("RGB: SPLASH"), false);
                  break;
            default:
                oled_write_ln_P(PSTR("RGB: OTHER"), false);
                break;
    }
}

bool oled_task_user(void) {
    oled_write_P(PSTR("KN: "), false);
    oled_write_ln(name, false);
    oled_write_P(PSTR("Layer:"), false);
    oled_write_ln(get_u8_str(get_highest_layer(layer_state), ' '), false);     
    render_rgb_status();
    return false;
}
