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

// Defines names for use in layer keycodes and the keymap
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    C(KC_Z),   KC_1,    KC_2,     KC_3,    KC_4,          KC_5, 
    KC_F24,    KC_6,    KC_7,     KC_8,    KC_9,          KC_0,
    C(KC_Y),   KC_BSPC, KC_QUOT,  KC_SPC,  SFT_T(KC_ENT), MO(1)                      
  ),
 
  [1] = LAYOUT(
    KC_PGUP,      _______,     _______,    KC_UP,      KC_LBRC,    KC_RBRC,  
    KC_F24,       LCTL(KC_A),  KC_LEFT,    KC_DOWN,    KC_RGHT,    LCTL(KC_S),
    KC_PGDOWN,    KC_ESC,      KC_COMMA,   KC_DOT,     _______,    _______
  ), 

  [2] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),

  [3] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
    
  [4] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [5] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [6] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [7] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [8] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [9] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [10] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [11] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [12] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [13] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [14] = LAYOUT(
    _______, _______,    _______,    _______,    _______,    _______,     
    KC_F24,   _______,    _______,    _______,    _______,    _______,
    _______, _______,    _______,    _______,    _______,    _______
  ),
  
  [15] = LAYOUT(
    RGB_RMOD, RGB_VAI,    RGB_SAI,    RGB_HUI,    RGB_TOG,    _______,  
    KC_F24,   RGB_VAD,    RGB_SAD,    RGB_HUD,    _______,   _______,
    RGB_MOD,_______,    _______,    _______,    _______,    _______
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

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    }

    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_F24:
      if (record->event.pressed) {
        if (get_highest_layer(layer_state|default_layer_state) == 15 ) {
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
  {  0, 1, 2, 3 },
  {  4, 5, 6, 7 },
  {  8, 9, 10, 11 },
  {  12, 13, 14, 15 }
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
  2, 2, 2, 2, 2,
  2, 2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {    
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
        switch(get_highest_layer(layer_state|default_layer_state)) {
            case 0:
                rgb_matrix_set_color(i, RGB_MAGENTA);
                break;
            case 1:
                rgb_matrix_set_color(i, RGB_CYAN);
                break;
            case 2:
                rgb_matrix_set_color(i, RGB_GREEN);
                break;
            case 3:
                rgb_matrix_set_color(i, RGB_AZURE);
                break;
            case 4:
                rgb_matrix_set_color(i, RGB_BLUE);
                break;
            case 5:
                rgb_matrix_set_color(i, RGB_CHARTREUSE);
                break;
            case 6:
                rgb_matrix_set_color(i, RGB_CORAL);
                break;
            case 7:
                rgb_matrix_set_color(i, RGB_GOLD);
                break;
            case 8:
                rgb_matrix_set_color(i, RGB_PINK);
                break;
            case 9:
                rgb_matrix_set_color(i, RGB_GOLDENROD);
                break;
            case 10:
                rgb_matrix_set_color(i, RGB_SPRINGGREEN);
                break;
            case 11:
                rgb_matrix_set_color(i, RGB_TEAL);
                break;
            case 12:
                rgb_matrix_set_color(i, RGB_TURQUOISE);
                break;
            case 13:
                rgb_matrix_set_color(i, RGB_WHITE);
                break;
            case 14:
                rgb_matrix_set_color(i, RGB_RED);
                break;
            default:
                rgb_matrix_set_color(i, RGB_YELLOW);
                break;
        }
      }
    }
};
