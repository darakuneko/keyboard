/* Copyright 2022 daraku-neko
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
#include "bongo.h"


#define STEP 30
uint8_t kp = 0;
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    LGUI_T(KC_ESC),    KC_1,            KC_2,            KC_3,            KC_4,     KC_5,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_6,    KC_7,    KC_8,             KC_9,         KC_0,    RGUI_T(KC_GRV),
    LCTL_T(KC_TAB),    KC_Q,            KC_W,            KC_E,            KC_R,     KC_T,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,       KC_Y,    KC_U,    KC_I,             KC_O,         KC_P,    RCTL_T(KC_BSLS), 
    LSFT_T(KC_DEL),    KC_A,            KC_S,            KC_D,            KC_F,     KC_G,                                                              KC_H,    KC_J,    KC_K,             KC_L,         KC_SCLN, RSFT_T(KC_QUOT),
                       KC_Z,            KC_X,            KC_C,            KC_V,     KC_B,                                                              KC_N,    KC_M,    KC_COMM,          KC_DOT,       KC_SLSH, 
                                        KC_LBRC,         LALT_T(KC_RBRC), MO(1),    LCTL(KC_V),                                                        RCTL(KC_X), KC_BSPC, RALT_T(KC_MINS),  KC_EQL,
                                                                          KC_SPC,   LCTL(KC_C),                                                        RCTL(KC_A), KC_ENT, 
                                                                                                    LCTL(KC_Z), KC_F24, RCTL(KC_Y)
  ),

  [1] = LAYOUT(
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    XXXXXXX,    XXXXXXX,
               XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
                           XXXXXXX,   XXXXXXX,   KC_TRNS,    KC_TRNS,                                                                     KC_TRNS,    KC_TRNS,    XXXXXXX,    XXXXXXX,
                                                 KC_TRNS,    KC_TRNS,                                                                     KC_TRNS,    KC_TRNS, 
                                                                                      LCTL(KC_Z), KC_F24, RCTL(KC_Y)
 ),

  [2] = LAYOUT(
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
               XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
                           XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                                                 XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX, 
                                                                                        XXXXXXX, KC_F24, XXXXXXX
  ),

  [3] = LAYOUT(
    LGUI_T(KC_ESC),    KC_1,            KC_2,            KC_3,            KC_4,     KC_5,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_6,    KC_7,    KC_8,             KC_9,         KC_0,    RGUI_T(KC_GRV),
    LCTL_T(KC_TAB),    KC_Q,            KC_W,            KC_E,            KC_R,     KC_T,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,       KC_Y,    KC_U,    KC_I,             KC_O,         KC_P,    RCTL_T(KC_BSLS), 
    LSFT_T(KC_DEL),    KC_A,            KC_S,            KC_D,            KC_F,     KC_G,                                                              KC_H,    KC_J,    KC_K,             KC_L,         KC_SCLN, RSFT_T(KC_QUOT),
                       KC_Z,            KC_X,            KC_C,            KC_V,     KC_B,                                                              KC_N,    KC_M,    KC_COMM,          KC_DOT,       KC_SLSH, 
                                        KC_LBRC,         LALT_T(KC_RBRC), MO(4),    LCTL(KC_V),                                                        RCTL(KC_X), KC_BSPC, RALT_T(KC_MINS),  KC_EQL,
                                                                          KC_SPC,   LCTL(KC_C),                                                        RCTL(KC_Q), KC_ENT, 
                                                                                                    LCTL(KC_Z), KC_F24, RCTL(KC_Y)
  ),

  [4] = LAYOUT(
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    KC_TRNS,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    XXXXXXX,    XXXXXXX,
               XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
                           XXXXXXX,   XXXXXXX,   KC_TRNS,    KC_TRNS,                                                                     KC_TRNS,    KC_TRNS,    XXXXXXX,    XXXXXXX,
                                                 KC_TRNS,    KC_TRNS,                                                                     KC_TRNS,    KC_TRNS, 
                                                                                      LCTL(KC_Z), KC_F24, RCTL(KC_Y)
  ),

  [5] = LAYOUT(
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,   XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
    XXXXXXX,   XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
               XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
                           XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                                                 XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX, 
                                                                                        XXXXXXX, KC_F24, XXXXXXX
  ),

  [6] = LAYOUT(
    RGB_VAI,   RGB_SAI,    RGB_HUI,   RGB_SPI,   RGB_MOD,    RGB_TOG,   QK_BOOT,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    RGB_VAD,   RGB_SAD,    RGB_HUD,   RGB_SPD,   RGB_RMOD,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,  XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
    XXXXXXX,   DT_PRNT,    DT_UP,     DT_DOWN,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
               XXXXXXX,    XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX, 
                           XXXXXXX,   XXXXXXX,   XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX,    XXXXXXX,    XXXXXXX,
                                                 XXXXXXX,    XXXXXXX,                                                                     XXXXXXX,    XXXXXXX, 
                                                                                        XXXXXXX, KC_F24, XXXXXXX
  )

};

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 8, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 8, .col = 1},
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
    kp = (kp + 1) % STEP;
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
    case RGB_MODE_FORWARD:
      if (record->event.pressed) {
        rgblight_step();
      }
    return false;
   case RGB_MODE_REVERSE:
      if (record->event.pressed) {
        rgblight_step_reverse();
      }
    return false;
    default:
      return true;
  }
};

 
led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { 0,   8,   16,   24,   32,   40,   48,   56,   64,}, \
    { 1,   9,   17,   25,   33,   41,   49,   57,   65,}, \
    { 2,   10,   18,   26,   34,   42,   50,   58,   66,}, \
    { 3,   11,   19,   27,   35,   43,   51,   59,   67,}, \
    { 4,   12,   20,   28,   36,   44,   52,   60,   68,}, \
    { 5,   13,   21,   29,   37,   45,   53,   61,   69,}, \
    { 6,   14,   22,   30,   38,   46,   54,   62,   70,}, \
    { 7,   15,   23,   31,   39,   47,   55,   63,   NO_LED,}, \
    { NO_LED,   NO_LED,   NO_LED,   NO_LED,   NO_LED,   NO_LED,   NO_LED,   NO_LED,   NO_LED} \

}, {
  // LED Index to Physical Position 
  {0, 0},    {13, 0},    {26, 0},    {39, 0},    {52, 0},    {65, 0},    {78, 0},    {91, 0},    {104, 0},    {117, 0},    {130, 0},   {143, 0},   {156, 0},   {169, 0},   {182, 0},   {195, 0},   {208, 0},   {224, 0},
  {0, 13},   {13, 13},   {26, 13},   {39, 13},   {52, 13},   {65, 13},   {78, 13},   {91, 13},   {104, 13},   {117, 13},   {130, 13},  {143, 13},  {156, 13},  {169, 13},  {182, 13},  {195, 13},  {208, 13},  {224, 13},
  {0, 26},   {13, 26},   {26, 26},   {39, 26},   {52, 26},   {65, 26},                                                                             {156, 26},  {169, 26},  {182, 26},  {195, 26},  {208, 26},  {224, 26},
             {13, 39},   {26, 39},   {39, 39},   {52, 39},   {65, 39},                                                                             {156, 39},  {169, 39},  {182, 39},  {195, 39},  {208, 39},  
                                     {26, 52},   {39, 52},   {65, 52},   {78, 52},                                                     {143, 52},  {156, 52},  {182, 52},  {195, 52},   
                                                             {65, 64},   {78, 64},                                                     {143, 64},  {156, 64},  
  }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4,                   4, 4, 4, 4, 4, 4,
     4, 4, 4, 4, 4,                   4, 4, 4, 4, 4,
        4, 4, 4, 4,                   4, 4, 4, 4,
              4, 4,                   4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
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

void render_rgb_status(void) {

    switch(rgb_matrix_config.mode) {
            case 0:
                oled_write_ln(PSTR("RGB: NONE"), false);
                break;
            case 1:
                oled_write_ln(PSTR("RGB: SOLID_COLOR"), false);
                break;        
            case 2:
                oled_write_ln(PSTR("ALPHAS_MODS"), false);
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
  render_anim();  // renders pixelart

  oled_set_cursor(0, 0);  
  oled_write(PSTR("WPM:"), false);
  oled_write(get_u8_str(get_current_wpm(), ' '), false);     
  oled_set_cursor(0, 1);  
  oled_write("LYR:", false);
  oled_write(get_u8_str(get_highest_layer(layer_state), ' '), false); 
  oled_write(" ", false);
  static char l1[] = "                \x94\x95\x96\x97";
  static char l2[] = "                 \xB4\xB5\xB6\xB7";
  static char r1[] = "                \x98\x99\x9A\x9B";
  static char r2[] = "                 \xB8\xB9\xBA\xBB";
  oled_set_cursor(0, 4);  
  int space = kp % STEP;
  if (space > STEP / 2) space = STEP - space;
  if (kp < STEP / 2) {
    oled_write(l1 + space, false);
    oled_write(l2, false);
  } else {
    oled_write(r1 + space, false);
    oled_write(r2, false);     
  }
  oled_write_ln("", false);
  oled_write_P(PSTR("KEY: "), false);
  oled_write_ln(name, false);  
  render_rgb_status();
  return false;
}
