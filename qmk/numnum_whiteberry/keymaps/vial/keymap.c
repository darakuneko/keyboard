/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H

enum {
  U_Layer_UP = QK_KB_0,
  U_Layer_DOWN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
		KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
		KC_LGUI, KC_LSFT, KC_LALT, KC_BSPC, KC_DEL,
		KC_MPLY, U_Layer_UP
	),
	[1] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[2] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[3] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[4] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[5] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[6] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	),
	[7] = LAYOUT(
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_NO, U_Layer_UP
	)

};

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
	[0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
	[1] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[2] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[3] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[4] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[5] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[6] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
	[7] = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_NO, KC_NO) },
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
	if(rgb_matrix_get_mode() == 1){
    	int current_layer = get_highest_layer(layer_state|default_layer_state);  
    	HSV hsv = {0, 255, rgblight_get_val()};
    	if (current_layer == 1) {
      		hsv.h = 213; //MAGENTA
    	} else if (current_layer == 2)  {
      		hsv.h = 85; //GREEN
    	} else if (current_layer == 3)  {
      		hsv.h = 170; //BLUE
    	} else if (current_layer == 4)  {
      		hsv.h = 11; //CORAL
      		hsv.s = 176;
    	} else if (current_layer == 5)  {
      		hsv.h = 43; //YELLOW
    	} else if (current_layer == 6)  {
      		hsv.h = 255; //RED
    	} else if (current_layer == 7)  {
      		hsv.h = 0; //White
			hsv.s = 0;
    	} else {
      		hsv.h = 128; //CYAN
    	}
    	RGB rgb = hsv_to_rgb(hsv);
    	for (uint8_t i = led_min; i <= led_max; i++) {
        	if (HAS_FLAGS(g_led_config.flags[i], 0x04)) {
          		rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        	}
    	}
	}

    return false;
};

int end_layer = 7;
int lasted_layer;

void layer_up(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == end_layer) {
    layer_clear();
  } else {
    layer_move(layer + 1); 
  }
} 

void layer_down(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == 0) {
    layer_move(end_layer);
  } else {
    layer_move(layer - 1); 
  }
}  

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case U_Layer_UP:
      if (record->event.pressed) {    
        layer_up();
      }
      return false;   
    case U_Layer_DOWN:
      if (record->event.pressed) {    
        layer_down();
      }
      return false;  
    default:
      return true;
  }
}