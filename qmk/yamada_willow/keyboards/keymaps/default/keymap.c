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
    KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,                   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_GRV,
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,                   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_BSLS, 
    KC_LCTL, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,                   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B, KC_LBRC, KC_RBRC, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                 KC_LGUI, KC_LALT, MO(1),   KC_SPC,                    KC_ENT,     KC_BSPC, KC_RCTL, KC_RALT,
    C(KC_R),  KC_END,   C(KC_V),
    KC_HOME,  KC_F23,    KC_PGDN,   S(KC_EQL),
    C(KC_C),  KC_PGUP,    C(KC_A),   KC_EQL,
    KC_MINS,  S(KC_MINS),  
    KC_F24,   C(KC_Y), C(KC_Z),  KC_F24,  C(KC_PMNS), C(KC_PPLS), KC_F24, RGB_RMOD, RGB_MOD
    ),

[1] = LAYOUT(
    RESET, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    RGB_VAI, RGB_SAI,    RGB_HUI,    RGB_MOD,   RGB_TOG,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    RGB_VAD, RGB_SAD,    RGB_HUD,    RGB_RMOD,  XXXXXXX,   XXXXXXX,                   KC_LEFT,   KC_DOWN,   KC_UP,      KC_RGHT,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                               XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX,   KC_DEL,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX, 
    KC_F24,   RGB_HUD, RGB_HUI,  KC_F24,  RGB_SAD, RGB_SAI, KC_F24, RGB_RMOD, RGB_MOD
    ),

[2] = LAYOUT(
    KC_ESC,  KC_1,    KC_2,    KC_3,   KC_4,   KC_5,                   KC_6,   KC_7,   KC_8,    KC_9,    KC_0,    KC_GRV,
    KC_TAB,  KC_Q,    KC_W,    KC_E,   KC_R,   KC_T,                   KC_Y,   KC_U,   KC_I,    KC_O,    KC_P,    KC_BSLS, 
    KC_LCTL, KC_A,    KC_S,    KC_D,   KC_F,   KC_G,                   KC_H,   KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
    KC_LSFT, KC_Z,    KC_X,    KC_C,   KC_V,   KC_B, KC_LBRC, KC_RBRC, KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
                 KC_LGUI, KC_LALT, MO(3),   KC_SPC,                    KC_ENT,     KC_BSPC, KC_RCTL, KC_RALT,
    C(KC_R),  KC_END,   C(KC_V),
    KC_HOME,  KC_F23,    KC_PGDN,   S(KC_EQL),
    C(KC_C),  KC_PGUP,    C(KC_A),   KC_EQL,
    KC_MINS,  S(KC_MINS),  
    KC_F24,   C(KC_Y), C(KC_Z),  KC_F24,  C(KC_PMNS), C(KC_PPLS), KC_F24, RGB_RMOD, RGB_MOD
    ),

[3] = LAYOUT(
    RESET, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    RGB_VAI, RGB_SAI,    RGB_HUI,    RGB_MOD,   RGB_TOG,   XXXXXXX,                   XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    RGB_VAD, RGB_SAD,    RGB_HUD,    RGB_RMOD,  XXXXXXX,   XXXXXXX,                   KC_LEFT,   KC_DOWN,   KC_UP,      KC_RGHT,    XXXXXXX,    XXXXXXX,
    XXXXXXX, XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                               XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX,                   XXXXXXX,   KC_DEL,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX,    XXXXXXX,   XXXXXXX,
    XXXXXXX,    XXXXXXX, 
    KC_F24,   RGB_HUD, RGB_HUI,  KC_F24,  RGB_SAD, RGB_SAI, KC_F24, RGB_RMOD, RGB_MOD
    ),    
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 1, .col = 9},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 2, .col = 9},
    .pressed = false
}; 

keyevent_t encoder2_ccw = {
    .key = (keypos_t){.row = 3, .col = 9},
    .pressed = false
};

keyevent_t encoder2_cw = {
    .key = (keypos_t){.row = 4, .col = 9},
    .pressed = false
}; 

keyevent_t encoder3_ccw = {
    .key = (keypos_t){.row = 5, .col = 9},
    .pressed = false
};

keyevent_t encoder3_cw = {
    .key = (keypos_t){.row = 6, .col = 9},
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
        if (get_highest_layer(layer_state|default_layer_state) == 3 ) {
          layer_clear();
        } else {
          layer_invert(get_highest_layer(layer_state)+1); 
        }
      } 
      return false;
    case KC_F23:
      if (record->event.pressed) {
        rgb_matrix_mode(41);
      } 
      return false;      
    default:
      return true;
  }
};
 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { NO_LED, 9,      18,     27,     36,     45,     54,     NO_LED, NO_LED, NO_LED, },
    { 0,      NO_LED, 19,     28,     37,     46,     55,     NO_LED, NO_LED, NO_LED, }, 
    { 1,      10,     NO_LED, 29,     38,     47,     56,     NO_LED, NO_LED, NO_LED, }, 
    { 2,      11,     20,     NO_LED, 39,     48,     57,     NO_LED, NO_LED, NO_LED, }, 
    { 3,      12,     21,     30,     NO_LED, 49,     58,     NO_LED, NO_LED, NO_LED, }, 
    { 4,      13,     22,     31,     40,     NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, }, 
    { 5,      14,     23,     32,     41,     50,     NO_LED, NO_LED, NO_LED, NO_LED, }, 
    { 6,      15,     24,     33,     42,     51,     NO_LED, NO_LED, NO_LED, NO_LED, }, 
    { 7,      16,     25,     34,     43,     52,     NO_LED, NO_LED, NO_LED, NO_LED, }, 
    { 8,      17,     26,     35,     44,     53,     NO_LED, NO_LED, NO_LED, NO_LED  }, 
}, {
  // LED Index to Physical Position
  {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0}, 
  {0, 16}, {20, 16}, {40, 16}, {60, 16}, {80, 16}, {100, 16}, {120, 16}, {140, 16}, {160, 16}, {180, 16}, {200, 16}, {220, 16},
  {0, 32}, {20, 32}, {40, 32}, {60, 32}, {80, 32}, {100, 32}, {120, 32}, {140, 32}, {160, 32}, {180, 32}, {200, 32}, {220, 32},
  {0, 48}, {17, 48}, {34, 48}, {51, 48}, {68, 48}, {85,  48}, {102, 48}, {119, 48}, {138, 48}, {155, 48}, {172, 48}, {189, 48}, {189, 48}, {206, 48}, {224, 48},
  {0, 32}, {20, 64}, {40, 96}, {60, 128}, {80, 160}, {100, 192}, {120, 224}
}, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1,
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
    } else {
      hsv.h = 191; //PURPLE
    }

    RGB rgb = hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i <= led_max; i++) {
        if ((HAS_FLAGS(g_led_config.flags[i], 0x01) || HAS_FLAGS(g_led_config.flags[i], 0x02)) && rgb_matrix_config.mode != 41) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};
