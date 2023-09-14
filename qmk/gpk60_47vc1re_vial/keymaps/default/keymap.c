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
#include "code2name.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( 
      KC_ESC,                KC_Q,    KC_W,    KC_E,      KC_R,              KC_T,            KC_Y,       KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,
      MT(MOD_LCTL,KC_TAB),   KC_A,    KC_S,    KC_D,      KC_F,              KC_G,            KC_H,       KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,
      KC_LSFT,               KC_Z,    KC_X,    KC_C,      KC_V,              KC_B,            KC_N,       KC_M,    KC_COMM, KC_DOT,  KC_RSFT, MO(2),
      KC_LGUI,               KC_LALT, C(KC_PMNS), C(KC_PPLS),   LT(1, KC_SPC),     LT(1,KC_BSPC),   KC_DEL,  KC_RCTL, KC_RALT, KC_RGUI,
      KC_F24,   C(KC_Z), C(KC_Y)
  ),
 
  [1] = LAYOUT(
    KC_GRV,    S(KC_1),   S(KC_2),   S(KC_3),   S(KC_4),   S(KC_5),   S(KC_6),   S(KC_7),     S(KC_8),   S(KC_9),    S(KC_0),  S(KC_MINS), S(KC_EQL), 
    C(KC_SPC),     KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LEFT,   KC_DOWN,     KC_UP,     KC_RGHT,    KC_MINS,  KC_EQL,
    KC_NO,     KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_QUOT,   S(KC_QUOT),  KC_SLSH,   S(KC_SLSH),  KC_BSLS, S(KC_BSLS),
    KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,  
    KC_F24,   C(KC_Z), C(KC_Y)
  ),

  [2] = LAYOUT(
    KC_NO,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,   KC_F12, 
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,  
    KC_F24, C(KC_Z), C(KC_Y)
  ),
  
  [3] = LAYOUT(
      KC_ESC,                KC_Q,    KC_W,    KC_E,      KC_R,              KC_T,            KC_Y,       KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,
      MT(MOD_LCTL,KC_TAB),   KC_A,    KC_S,    KC_D,      KC_F,              KC_G,            KC_H,       KC_J,    KC_K,    KC_L,    KC_SCLN, KC_ENT,
      KC_LSFT,               KC_Z,    KC_X,    KC_C,      KC_V,              KC_B,            KC_N,       KC_M,    KC_COMM, KC_DOT,  KC_RSFT, MO(5),
      KC_LGUI,               KC_LALT, C(KC_PMNS), C(KC_PPLS),   LT(4, KC_SPC),     LT(4,KC_BSPC),   KC_DEL,  KC_RCTL, KC_RALT, KC_RGUI,
      KC_F24,   C(KC_Z), C(KC_Y)
  ),

  [4] = LAYOUT(
    KC_GRV,    S(KC_1),   S(KC_2),   S(KC_3),   S(KC_4),   S(KC_5),   S(KC_6),   S(KC_7),     S(KC_8),   S(KC_9),    S(KC_0),  S(KC_MINS), S(KC_EQL), 
    C(KC_SPC),     KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_LEFT,   KC_DOWN,     KC_UP,     KC_RGHT,    KC_MINS,  KC_EQL,
    KC_NO,     KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_QUOT,   S(KC_QUOT),  KC_SLSH,   S(KC_SLSH),  KC_BSLS, S(KC_BSLS),
    KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,  
    KC_F24,   C(KC_Z), C(KC_Y)
  ),

  [5] = LAYOUT(
    KC_NO,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,    KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,    KC_F11,   KC_F12, 
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,     KC_NO,
    KC_NO,        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,  
    KC_F24, C(KC_Z), C(KC_Y)
  ),

  [6] = LAYOUT(
    RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_SPI,   RGB_MOD,     RGB_TOG,   KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO, QK_BOOT,
    RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_SPD,   RGB_RMOD,    KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    DT_PRNT,  DT_UP,      DT_DOWN,    KC_NO,     KC_NO,       KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_NO,    KC_NO,      KC_NO,      KC_NO,     KC_NO,       KC_NO,     KC_NO,    KC_NO,    KC_NO,    KC_NO,
    KC_F24,     RGB_RMOD, RGB_MOD
  )
};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 7, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 7, .col = 1},
    .pressed = false
}; 

void matrix_scan_user(void) {
    if (encoder1_ccw.pressed) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (encoder1_cw.pressed) {
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
    { 0, 1, 2, 3, 4, 5, 6,  }, 
    { 7, 8, 9, 10, 11, 12, 13,  }, 
    { 14, 15, 16, 17, 18, 19, 20,  }, 
    { 21, 22, 23, 24, 25, 26, 27,  }, 
    { 28, 29, 30, 31, 32, 33, 34,  }, 
    { 35, 36, 37, 38, 39, 40, 41,  }, 
    { 42, 43, 44, 45, 46, NO_LED, NO_LED,}, 
    { NO_LED, NO_LED, NO_LED, NO_LED, NO_LED, NO_LED } 
}, {
  // LED Index to Physical Position
    {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0}, {224, 0},
    {0, 21}, {20, 21}, {40, 21}, {60, 21}, {80, 21}, {100, 21}, {120, 21}, {140, 21}, {160, 21}, {180, 21}, {200, 21}, {220, 21},
    {0, 42}, {20, 42}, {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42},
    {0, 63}, {20, 63}, {40, 63}, {60, 63}, {80, 63}, {100, 63}, {120, 63}, {140, 63}, {160, 63}, {180, 63}
    }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  } 
};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_val = rgb_matrix_get_val();

    //Lower than the value of RGB_MATRIX_MAXIMUM_BRIGHTNESS, if changed.
    if(current_val > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
      rgb_matrix_decrease_val();
      rgb_matrix_increase_val();
    }
    
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, current_val};
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
    return false;
};

void render_rgb_status(void) {
    switch(rgb_matrix_config.mode) {
            case 0:
                oled_write_ln(PSTR("RGB: NONE"), false);
                break;
            case 1:
                oled_write_ln(PSTR("RGB: SOLID_COLOR"), false);
                break;        
            case 2:
                oled_write_ln(PSTR("RGB: ALPHAS_MODS"), false);
                break;
            case 3:
                oled_write_ln(PSTR("RGB: GRADIENT_UPDN"), false);
                break;
            case 4:
                oled_write_ln(PSTR("RGB: GRADIENT_LR"), false);
                break;
            case 5:
                oled_write_ln(PSTR("RGB: BREATHING"), false);
                break;
            case 6:
                oled_write_ln(PSTR("RGB: BAND_SAT"), false);
                break;
            case 7:
                oled_write_ln(PSTR("RGB: BAND_VAL"), false);
                break;
            case 8:
                oled_write_ln(PSTR("RGB: BAND_PIN_SAT"), false);
                break;
            case 9:
                oled_write_ln(PSTR("RGB: BAND_PIN_VAL"), false);
                break;
            case 10:
                oled_write_ln(PSTR("RGB: BAND_SPRL_SAT"), false);
                break;
            case 11:
                oled_write_ln(PSTR("RGB: BAND_SPRL_VAL"), false);
                break;
            case 12:
                oled_write_ln(PSTR("RGB: CYCLE_ALL"), false);
                break;
            case 13:
                oled_write_ln(PSTR("RGB: CYCLE_LR"), false);
                break;
            case 14:
                oled_write_ln(PSTR("RGB: CYCLE_UP_DOWN"), false);
                break;
            case 15:
                oled_write_ln(PSTR("RGB: RAINBOW_MOVING"), false);
                break;
            case 16:
                oled_write_ln(PSTR("RGB: CYCLE_OUT_IN"), false);
                break;
            case 17:
                oled_write_ln(PSTR("RGB: CYCLE_OUT_IN_D"), false);
                break;
            case 18:
                oled_write_ln(PSTR("RGB: CYCLE_PINWHEEL"), false);
                break;
            case 19:
                oled_write_ln(PSTR("RGB: CYCLE_SPIRAL"), false);
                break;
            case 20:
                oled_write_ln(PSTR("RGB: DUAL_BEACON"), false);
                break;
            case 21:
                oled_write_ln(PSTR("RGB: RAINBOW_BEACON"), false);
                break;
            case 22:
                oled_write_ln(PSTR("RGB: RAINBOW_PINWHLS"), false);
                break;
            case 23:
                oled_write_ln(PSTR("RGB: RAINDROPS"), false);
                break;                
            case 24:
                oled_write_ln(PSTR("RGB: JELLYBEAN"), false);
                break;
            case 25:
                oled_write_ln(PSTR("RGB: HUE_BREATHING"), false);
                break;                
            case 26:
                oled_write_ln(PSTR("RGB: HUE_PENDULUM"), false);
                break;
            case 27:
                oled_write_ln(PSTR("RGB: HUE_WAVE"), false);
                break;
            case 28:
                oled_write_ln(PSTR("RGB: PIXEL_FRACTAL"), false);
                break;
            case 29:
                oled_write_ln(PSTR("RGB: PIXEL_FLOW"), false);
                break;
            case 30:
                oled_write_ln(PSTR("RGB: PIXEL_RAIN"), false);
                break;
            case 31:
                oled_write_ln(PSTR("RGB: TYPING_HEATMAP"), false);
                break;
            case 32:
                oled_write_ln(PSTR("RGB: DIGITAL_RAIN"), false);
                break;
            case 33:
                oled_write_ln(PSTR("RGB: SLD_RCT_SIMPLE"), false);
                break;
            case 34:
                oled_write_ln(PSTR("RGB: SOLID_REACTIVE"), false);
                break;
            case 35:
                oled_write_ln(PSTR("RGB: SLD_RCT_WIDE"), false);
                break;
            case 36:
                oled_write_ln(PSTR("RGB: SLD_RCT_MWIDE"), false);
                break;
            case 37:
                oled_write_ln(PSTR("RGB: SLD_RCT_CROSS"), false);
                break;
            case 38:
                oled_write_ln(PSTR("RGB: SLD_RCT_MCROSS"), false);
                break;
            case 39:
                oled_write_ln(PSTR("RGB: SLD_RCT_NEXUS"), false);
                break;
            case 40:
                oled_write_ln(PSTR("RGB: SLD_RCT_MNEXUS"), false);
                break;
            case 41:
                oled_write_ln(PSTR("RGB: SPLASH"), false);
                break;
            case 42:
                oled_write_ln(PSTR("RGB: MULTISPLASH"), false);
                break;
            case 43:
                oled_write_ln(PSTR("RGB: SOLID_SPLASH"), false);
                break;
            case 44:
                oled_write_ln(PSTR("RGB: SOLID_MPLASH"), false);
                break;
            default:
                oled_write_ln(PSTR("RGB: OTHER"), false);
                break;
    }
}

bool oled_task_user(void) {
    oled_write_P(PSTR("KN: "), false);
    oled_write_ln(key_name, false);
    oled_write_P(PSTR("Layer:"), false);
    oled_write_ln(get_u8_str(get_highest_layer(layer_state), ' '), false);     
    render_rgb_status();
    return false;
}
