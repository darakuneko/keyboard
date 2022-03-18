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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[0] = LAYOUT(
    KC_ESC,  KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,      KC_HOME, KC_END,  KC_PMNS,
    KC_GRV,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,      KC_PSLS, KC_PAST, KC_PPLS, 
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,     KC_P7,   KC_P8,   KC_P9, 
    KC_LCTL, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_BSLS,     KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_LGUI, KC_RSFT,     KC_P1,   KC_P2,   KC_P3,
    MO(2),   KC_LGUI, KC_LALT, KC_DEL, LT(2, KC_SPC),          KC_ENT,     KC_BSPC, KC_RCTL, KC_RALT, C(KC_Y), KC_LGUI,     KC_P0,   KC_PDOT, KC_ENT,
    KC_F24,  C(KC_Z), C(KC_Y),  KC_F24,  C(KC_PMNS), C(KC_PPLS), KC_F24, RGB_RMOD, RGB_MOD
    ),

[1] = LAYOUT(
    KC_ESC,  KC_F1,   KC_F2,   KC_F3,  KC_F4,  KC_F5,  KC_F6,  KC_F7,  KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,      KC_HOME, KC_END,  KC_PMNS,
    KC_GRV,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,      KC_PSLS, KC_PAST, KC_PPLS, 
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,     KC_P7,   KC_P8,   KC_P9, 
    KC_LCTL, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_BSLS,     KC_P4,   KC_P5,   KC_P6,
    KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B,   KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_LGUI, KC_RSFT,     KC_P1,   KC_P2,   KC_P3,
    MO(2),   KC_LGUI, KC_LALT, KC_DEL, LT(2, KC_SPC),          KC_ENT,     KC_BSPC, KC_RCTL, KC_RALT, C(KC_Y), KC_LGUI,     KC_P0,   KC_PDOT, KC_ENT,
    KC_F24,  C(KC_Z), C(KC_Y),  KC_F24,  C(KC_PMNS), C(KC_PPLS), KC_F24, RGB_RMOD, RGB_MOD
    ),

[2] = LAYOUT(
    RESET, RGB_VAI, RGB_VAD, RGB_SAI, RGB_SAD, RGB_HUI, RGB_HUD, RGB_MOD, RGB_RMOD, RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSPC,    XXXXXXX, XXXXXXX, KC_PMNS,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX, KC_ENT,     XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, KC_UP,   XXXXXXX,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX,          XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    KC_LEFT, KC_DOWN, KC_RIGHT, 
    KC_F24,  C(KC_Z), C(KC_Y),  KC_F24,  C(KC_PMNS), C(KC_PPLS), KC_F24, RGB_RMOD, RGB_MOD
    ),

};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 10},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 5, .col = 10},
    .pressed = false
}; 

keyevent_t encoder2_ccw = {
    .key = (keypos_t){.row = 6, .col = 10},
    .pressed = false
};

keyevent_t encoder2_cw = {
    .key = (keypos_t){.row = 7, .col = 10},
    .pressed = false
}; 

keyevent_t encoder3_ccw = {
    .key = (keypos_t){.row = 8, .col = 10},
    .pressed = false
};

keyevent_t encoder3_cw = {
    .key = (keypos_t){.row = 9, .col = 10},
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

    if (IS_PRESSED(encoder2_ccw)) {
        encoder2_ccw.pressed = false;
        encoder2_ccw.time = (timer_read() | 1);
        action_exec(encoder2_ccw);
    }

    if (IS_PRESSED(encoder2_cw)) {
        encoder2_cw.pressed = false;
        encoder2_cw.time = (timer_read() | 1);
        action_exec(encoder2_cw);
    }
    
    if (IS_PRESSED(encoder3_ccw)) {
        encoder3_ccw.pressed = false;
        encoder3_ccw.time = (timer_read() | 1);
        action_exec(encoder3_ccw);
    }

    if (IS_PRESSED(encoder3_cw)) {
        encoder3_cw.pressed = false;
        encoder3_cw.time = (timer_read() | 1);
        action_exec(encoder3_cw);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    } else if (index == 1) {
        if (clockwise) {
            encoder2_cw.pressed = true; 
            encoder2_cw.time = (timer_read() | 1);
            action_exec(encoder2_cw);
        } else {
            encoder2_ccw.pressed = true;
            encoder2_ccw.time = (timer_read() | 1);
            action_exec(encoder2_ccw);
        }
    } else if (index == 2) {
        if (clockwise) {
            encoder3_cw.pressed = true; 
            encoder3_cw.time = (timer_read() | 1);
            action_exec(encoder3_cw);
        } else {
            encoder3_ccw.pressed = true;
            encoder3_ccw.time = (timer_read() | 1);
            action_exec(encoder3_ccw);
        }
    }

    return true;
}



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_F24:
      if (record->event.pressed) {
        if (get_highest_layer(layer_state|default_layer_state) == 2 ) {
          layer_clear();
        } else {
          layer_invert(get_highest_layer(layer_state)+1); 
        }
      } 
      return false;
    default:
      return true;
  }
};
 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { NO_LED, 10,   20,   30,   40,   50,   60,   70,   80,   90,   93, },
    { 0,   NO_LED, 21,   31,   41,   51,   61,   71,   81,   91,   NO_LED, }, 
    { 1,   11,   NO_LED, 32,   42,   52,   62,   72,   82,   NO_LED, NO_LED, }, 
    { 2,   12,   22,   NO_LED, 43,   53,   63,   73,   83,   NO_LED, NO_LED, }, 
    { 3,   13,   23,   33,   NO_LED, 54,   64,   74,   84,   NO_LED, NO_LED,}, 
    { 4,   14,   24,   34,   44,   NO_LED, 65,   75,   85,   NO_LED, NO_LED,}, 
    { 5,   15,   25,   35,   45,   55,   NO_LED, 76,   86,   NO_LED, NO_LED, }, 
    { 6,   16,   26,   36,   46,   56,   66,   NO_LED, 87,   NO_LED, NO_LED, }, 
    { 7,   17,   27,   37,   47,   57,   67,   77,   NO_LED, NO_LED, NO_LED, }, 
    { 8,   18,   28,   38,   48,   58,   68,   78,   88,   NO_LED, NO_LED, }, 
    { 9,   19,   29,   39,   49,   59,   69,   79,   89,   92,   NO_LED  } 
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

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
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

