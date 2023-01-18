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
#include <string.h>
#include <printf.h>

#include <qp.h>
#include <color.h>
#include "pm10.qff.h"

painter_device_t lcd;

//static painter_image_handle_t my_image;
static painter_font_handle_t my_font;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_ESC,               KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,    
    MT(MOD_LCTL, KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
    KC_LSFT,              KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH),
                                   KC_LALT,          LT(1, KC_SPC),             LT(1, KC_BSPC),            KC_LGUI,
    KC_F24 
  ),

  [1] = LAYOUT(
    KC_GRV,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_MINS,  KC_EQL,    KC_LBRC,   KC_RBRC,
    KC_LCTL,    KC_1,    KC_2,     KC_3,     KC_4,    KC_5,     KC_LEFT, KC_DOWN, KC_UP,    KC_RIGHT,  KC_QUOT,
    MO(2),    KC_6,    KC_7,     KC_8,     KC_9,    KC_0,     KC_NO,   KC_SCLN,   KC_COMM,  KC_DOT,    KC_SLSH,
                                 KC_PGUP,         KC_LSFT,            KC_LSFT,            KC_PGDN,                                 
    KC_F24
  ),

  [2] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,   KC_F8,  KC_F9,  KC_F10,   KC_F11,  KC_F12,  
    KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,  KC_NO,  KC_NO,    KC_NO,
    KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,  KC_NO,  KC_NO,    KC_NO,
                                   KC_NO,         KC_NO,            KC_NO,            KC_NO,
    KC_F24  
  ),

  [3] = LAYOUT(
    KC_ESC,               KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,    
    MT(MOD_LCTL, KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN,
    KC_LSFT,              KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  SFT_T(KC_SLSH),
                                   KC_LALT,          LT(1, KC_SPC),             LT(1, KC_BSPC),            KC_LGUI,
    KC_F24 
  ),

  [4] = LAYOUT(
    KC_GRV,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_MINS,  KC_EQL,    KC_LBRC,   KC_RBRC,
    MO(2),    KC_1,    KC_2,     KC_3,     KC_4,    KC_5,     KC_LEFT, KC_DOWN,  KC_UP,    KC_RIGHT,  KC_QUOT,
    KC_NO,    KC_6,    KC_7,     KC_8,     KC_9,    KC_0,     KC_NO,   KC_NO,  KC_COMM,  KC_DOT,    KC_SLSH,
                                 KC_NO,         KC_LSFT,            KC_LSFT,            KC_DEL,                                
    KC_F24 
  ),

  [5] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,   KC_F8,  KC_F9,  KC_F10,   KC_F11,  KC_F12,  
    KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,  KC_NO,  KC_NO,    KC_NO,
    KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,  KC_NO,  KC_NO,    KC_NO,
                                   KC_NO,         KC_NO,            KC_NO,            KC_NO,                                 
    KC_F24 
  ),


  [6] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,   QK_BOOT,
    KC_NO,   RGB_VAI, RGB_SAI,  RGB_HUI,  RGB_SPI, RGB_TOG,  DT_PRNT, DT_UP,   DT_DOWN, KC_NO,  EE_CLR, 
    KC_NO,   RGB_VAD, RGB_SAD,  RGB_HUD,  RGB_SPD, KC_NO,    KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,
                                           KC_NO,  KC_NO,    KC_NO,   KC_NO,   
    KC_F24 
  ),
};
 

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, },
  { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,},
  { 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33,},
  { 34, 35, 36, 37, NO_LED },
}, {
  // LED Index to Physical Position
  {0, 0},  {20, 0},  {40, 0},  {60, 0},  {80, 0},  {100, 0},  {120, 0},  {140, 0},  {160, 0},  {180, 0},  {200, 0},  {220, 0},
  {0, 21}, {20, 21}, {40, 21}, {60, 21}, {80, 21}, {100, 21}, {120, 21}, {140, 21}, {160, 21}, {180, 21},      {210, 21},
      {10, 42},      {40, 42}, {60, 42}, {80, 42}, {100, 42}, {120, 42}, {140, 42}, {160, 42}, {180, 42}, {200, 42}, {220, 42},
                     {40, 63},           {80, 63},                       {140, 63},            {180, 63},  
}, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int curr_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgb_matrix_get_val()};
  if (curr_layer == 1) {
    hsv.h = 43; //YELLOW
  } else if (curr_layer == 2)  {
    hsv.h = 11; //CORAL
  } else if (curr_layer == 3)  {
    hsv.h = 191; //PURPLE
  } else if (curr_layer == 4)  {
    hsv.h = 85; //GREEN
  } else if (curr_layer == 5)  {
    hsv.h = 0; //RED
  } else if (curr_layer == 6)  {
    hsv.h = 0;
    hsv.s = 0; //WHITE
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);
 
  for (uint8_t i = led_min; i <= led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }

};

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case KC_F24:
      if (record->event.pressed) {    
        encoder_layer_up();
      }
      return false;
    default:
      return true;
  }
};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =  { ENCODER_CCW_CW(C(KC_Z),    C(KC_Y)) },
    [1] =  { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [2] =  { ENCODER_CCW_CW(KC_NO,      KC_NO) },
    [3] =  { ENCODER_CCW_CW(C(KC_Z),    C(KC_Y)) },
    [4] =  { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS )) },
    [5] =  { ENCODER_CCW_CW(KC_NO,      KC_NO) },
    [6] =  { ENCODER_CCW_CW(RGB_RMOD,   RGB_MOD) },
};

void qp_layer(int curr_hue, int curr_layer) {
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "LAYER: %d", curr_layer);
    int16_t width = qp_textwidth(my_font, buf);
    if(curr_layer == 6) {
      qp_drawtext(lcd, (160 - width), (120 - my_font->line_height), my_font, buf);
    } else {
      qp_drawtext_recolor(lcd, (160 - width), (120 - my_font->line_height), my_font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
    }
}

void keyboard_post_init_user(void) {
    // Initialise the LCD
  lcd = qp_st7789_make_spi_device(320, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 3, 3);
  qp_init(lcd, QP_ROTATION_270);
  qp_rect(lcd, 0, 0, 320, 240, HSV_BLACK, true);

  my_font = qp_load_font_mem(font_pm10);

  const char *kb = "GPK60-38R1RE";
  int16_t width = qp_textwidth(my_font, kb);
  qp_drawtext(lcd, (240 - width), (80 - my_font->line_height), my_font, kb);
  qp_layer(128, 0);
}

void qp_rgb(int curr_hue, int curr_layer, char c[]) {
  char buf[64] = {0};
  snprintf(buf, sizeof(buf), "RGB:%s", c);
  int16_t width = qp_textwidth(my_font, buf);
  if(curr_layer == 6) {
    qp_drawtext(lcd, (320 - width), (160 - my_font->line_height), my_font, buf);
  } else {
    qp_drawtext_recolor(lcd, (320 - width), (160 - my_font->line_height), my_font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
  }
}

void qp_rgb_matrix_name(int curr_hue, int curr_layer) {
    switch(rgb_matrix_config.mode) {
            case 0:
                qp_rgb(curr_hue, curr_layer, "NONE        ");
                break;
            case 1:
                qp_rgb(curr_hue, curr_layer, "SOLID_COLOR ");
                break;        
            case 2:
                qp_rgb(curr_hue, curr_layer, "ALPHAS_MODS ");
                break;
            case 3:
                qp_rgb(curr_hue, curr_layer, "GRDNT_UPDN  ");
                break;
            case 4:
                qp_rgb(curr_hue, curr_layer, "GRDNT_LR    ");
                break;
            case 5:
                qp_rgb(curr_hue, curr_layer, "BREATHING   ");
                break;
            case 6:
                qp_rgb(curr_hue, curr_layer, "BAND_SAT    ");
                break;
            case 7:
                qp_rgb(curr_hue, curr_layer, "BAND_VAL    ");
                break;
            case 8:
                qp_rgb(curr_hue, curr_layer, "BAND_PIN_SAT");
                break;
            case 9:
                qp_rgb(curr_hue, curr_layer, "BAND_PIN_VAL");
                break;
            case 10:
                qp_rgb(curr_hue, curr_layer, "BAND_SPRL_ST");
                break;
            case 11:
                qp_rgb(curr_hue, curr_layer, "BAND_SPRL_VL");
                break;
            case 12:
                qp_rgb(curr_hue, curr_layer, "CYCLE_ALL   ");
                break;
            case 13:
                qp_rgb(curr_hue, curr_layer, "CYCLE_LR    ");
                break;
            case 14:
                qp_rgb(curr_hue, curr_layer, "CYC_UP_DOWN ");
                break;
            case 15:
                qp_rgb(curr_hue, curr_layer, "RNBW_MOVING ");
                break;
            case 16:
                qp_rgb(curr_hue, curr_layer, "CYCLE_OUT_IN");
                break;
            case 17:
                qp_rgb(curr_hue, curr_layer, "CYC_OUT_IN_D");
                break;
            case 18:
                qp_rgb(curr_hue, curr_layer, "CYCLE_PINWHL");
                break;
            case 19:
                qp_rgb(curr_hue, curr_layer, "CYCLE_SPIRAL");
                break;
            case 20:
                qp_rgb(curr_hue, curr_layer, "DUAL_BEACON ");
                break;
            case 21:
                qp_rgb(curr_hue, curr_layer, "RNBW_BEACON ");
                break;
            case 22:
                qp_rgb(curr_hue, curr_layer, "RNBW_PINWHLS");
                break;
            case 23:
                qp_rgb(curr_hue, curr_layer, "RAINDROPS   ");
                break;                
            case 24:
                qp_rgb(curr_hue, curr_layer, "JELLYBEAN   ");
                break;
            case 25:
                qp_rgb(curr_hue, curr_layer, "HUE_BREATH  ");
                break;                
            case 26:
                qp_rgb(curr_hue, curr_layer, "HUE_PENDULUM");
                break;
            case 27:
                qp_rgb(curr_hue, curr_layer, "HUE_WAVE    ");
                break;
            case 28:
                qp_rgb(curr_hue, curr_layer, "PXL_FRACTAL ");
                break;
            case 29:
                qp_rgb(curr_hue, curr_layer, "PXL_FLOW    ");
                break;
            case 30:
                qp_rgb(curr_hue, curr_layer, "PXL_RAIN    ");
                break;
            case 31:
                qp_rgb(curr_hue, curr_layer, "TYPING_HTMAP");
                break;
            case 32:
                qp_rgb(curr_hue, curr_layer, "DIGITAL_RAIN");
                break;
            case 33:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_SMPL");
                break;
            case 34:
                qp_rgb(curr_hue, curr_layer, "SLD_REACTIVE");
                break;
            case 35:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_WIDE");
                break;
            case 36:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_MWDE");
                break;
            case 37:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_CRSS");
                break;
            case 38:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_MCRS");
                break;
            case 39:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_NEXS");
                break;
            case 40:
                qp_rgb(curr_hue, curr_layer, "SLD_RCT_MNXS");
                break;
            case 41:
                qp_rgb(curr_hue, curr_layer, "SPLASH      ");
                break;
            case 42:
                qp_rgb(curr_hue, curr_layer, "MULTISPLASH ");
                break;
            case 43:
                qp_rgb(curr_hue, curr_layer, "SOLID_SPLASH");
                break;
            case 44:
                qp_rgb(curr_hue, curr_layer, "SOLID_MPLASH");
                break;
            default:
                qp_rgb(curr_hue, curr_layer, "OTHER       ");
                break;
    }
}

bool wpm_keycode_user(uint16_t keycode) {
    return true;
}

void housekeeping_task_user() {
  bool            layer_redraw = false;
  static uint16_t last_layer       = 0;
  uint8_t         curr_layer       = get_highest_layer(layer_state|default_layer_state);
  if (last_layer != curr_layer) {
      last_layer       = curr_layer;
      layer_redraw = true;
  }


  bool            rgb_effect_redraw = false;
  static uint16_t last_effect       = 0xFFFF;
  uint8_t         curr_effect       = rgb_matrix_config.mode;
  if (last_effect != curr_effect) {
      last_effect       = curr_effect;
      rgb_effect_redraw = true;
  }

  bool            wpm_redraw      = false;
  static uint32_t last_wpm_update = 0;
  if (timer_elapsed32(last_wpm_update) > 125) {
    last_wpm_update = timer_read32();
    wpm_redraw      = true;
  }

  uint16_t curr_hue;
  if (curr_layer == 1) {
    curr_hue = 43; //YELLOW
  } else if (curr_layer == 2)  {
    curr_hue = 11; //CORAL
  } else if (curr_layer == 3)  {
    curr_hue = 200; //PURPLE
  } else if (curr_layer == 4)  {
    curr_hue = 85; //GREEN
  } else if (curr_layer == 5)  {
    curr_hue = 0; //RED
  } else if (curr_layer == 6)  {
    curr_hue = 255; //WHITE
  } else {
    curr_hue = 128; //CYAN
  }
  if (layer_redraw) {
    qp_layer(curr_hue, curr_layer);
  }
  if (layer_redraw || rgb_effect_redraw) {
    qp_rgb_matrix_name(curr_hue, curr_layer);
  }
  if (wpm_redraw) {
    char buf[64] = {0};
    snprintf(buf, sizeof(buf), "WPM: %d   ", (int)get_current_wpm());
    int16_t width = qp_textwidth(my_font, buf);
    int16_t widthL = strlen(buf) * 20;
    if(curr_layer == 6) {
        qp_drawtext(lcd, (widthL - width), (199 - my_font->line_height), my_font, buf);
    } else {
        qp_drawtext_recolor(lcd, (widthL - width), (199 - my_font->line_height), my_font, buf, curr_hue, 255, 255, curr_hue, 255, 0);
    }
  }
}