/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "code2name.c"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
		KC_LGUI, KC_LALT, MO(2),   MO(1),   LT(1,KC_SPC),  LT(1,KC_BSPC), KC_RALT, KC_RGUI, KC_F24
	),

	[1] = LAYOUT(
		KC_GRV,  KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_MINS, KC_EQL,  KC_NO,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_LSFT, KC_LSFT,   KC_LSFT, MO(3),   KC_NO,   KC_F24
	),

	[2] = LAYOUT(
		KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F24
	),

	[3] = LAYOUT(
		KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   EE_CLR,
		KC_NO,   KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_F24
	),

};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] =  { ENCODER_CCW_CW(C(KC_Z),    C(KC_Y)) },
    [1] =  { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [2] =  { ENCODER_CCW_CW(KC_NO,      KC_NO) },
    [3] =  { ENCODER_CCW_CW(RGB_RMOD,   RGB_MOD) },
};


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int curr_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgb_matrix_get_val()};
  if (curr_layer == 1) {
    hsv.h = 43; //YELLOW
  } else if (curr_layer == 2)  {
    hsv.h = 11; //CORAL
  } else if (curr_layer == 3)  {
    hsv.h = 191; //PURPLE
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);
 
  for (uint8_t i = led_min; i <= led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
  return false;
};

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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    set_code_to_name(keycode);
  }
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

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_180;
}

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
                oled_write_ln(PSTR("RGB: NONE"), false);
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
