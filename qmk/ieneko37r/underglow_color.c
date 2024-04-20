/* SPDX-License-Identifier: GPL-2.0-or-later */
#include "underglow_color.h"

typedef struct _hsv_layer_t {
    uint8_t hue : 8;
    uint8_t sat : 8;
    uint8_t val : 8;
	bool change : 1;
} hsv_layer_t;

typedef struct _hsv_layers_t {
    hsv_layer_t hsv_layer0;
	hsv_layer_t hsv_layer1;
	hsv_layer_t hsv_layer2;
	hsv_layer_t hsv_layer3;
} hsv_layers_t;

hsv_layers_t hsv_layers;

bool is_setting_hsv_layer;

enum {
  START_COLOR_LAYER = QK_KB_0,
  SET_COLOR_LAYER0,
  SET_COLOR_LAYER1,
  SET_COLOR_LAYER2,
  SET_COLOR_LAYER3
};

void init_hsv_layer_color(void) {
	eeconfig_read_user_datablock(&hsv_layers);
	is_setting_hsv_layer = false;
}

void update_hsv_layer(keyrecord_t *record, int layer_num) {
	if (record->event.pressed) {
		if(is_setting_hsv_layer){
			hsv_layer_t hsv;
			hsv.hue = rgblight_get_hue();
			hsv.sat = rgblight_get_sat();
			hsv.val = rgblight_get_val();
			hsv.change = true;
			if(layer_num == 1){
				hsv_layers.hsv_layer1 = hsv;
			} else if(layer_num == 2){
				hsv_layers.hsv_layer2 = hsv;
			} else if(layer_num == 3){
				hsv_layers.hsv_layer3 = hsv;
			} else {
				hsv_layers.hsv_layer0 = hsv;
			}
			eeconfig_update_user_datablock(&hsv_layers);
			is_setting_hsv_layer = false;
		}
	}
}

bool update_hsv_layer_color(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
	case START_COLOR_LAYER:
		if (record->event.pressed) {
			is_setting_hsv_layer = !is_setting_hsv_layer;
		}
        return false;
	case SET_COLOR_LAYER0:
		update_hsv_layer(record, 0);
        return false;
	case SET_COLOR_LAYER1:
		update_hsv_layer(record, 1);
        return false;
	case SET_COLOR_LAYER2:
		update_hsv_layer(record, 2);
        return false;
	case SET_COLOR_LAYER3:
		update_hsv_layer(record, 3);
        return false;
 	default:
    	return true;
  }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int current_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgblight_get_val()};
  
  if(is_setting_hsv_layer){
  	hsv = rgblight_get_hsv();
  } else{
	if (current_layer == 1) {
		if(!hsv_layers.hsv_layer1.change) {
			hsv.h = 191; //PURPLE
		} else {
			hsv.h = hsv_layers.hsv_layer1.hue;
			hsv.s = hsv_layers.hsv_layer1.sat;
			hsv.v = hsv_layers.hsv_layer1.val;
		}
  	} else if (current_layer == 2)  {
		if(!hsv_layers.hsv_layer2.change) {
    		hsv.h = 85; //GREEN
		} else {
			hsv.h = hsv_layers.hsv_layer2.hue;
			hsv.s = hsv_layers.hsv_layer2.sat;
			hsv.v = hsv_layers.hsv_layer2.val;
		}
  	} else if (current_layer == 3)  {
 		if(!hsv_layers.hsv_layer3.change) {
    		hsv.h = 43; //YELLOW
		} else {
			hsv.h = hsv_layers.hsv_layer3.hue;
			hsv.s = hsv_layers.hsv_layer3.sat;
			hsv.v = hsv_layers.hsv_layer3.val;
		}
  	} else {
 		if(!hsv_layers.hsv_layer0.change) {
			hsv.h = 128; //CYAN
		} else {
			hsv.h = hsv_layers.hsv_layer0.hue;
			hsv.s = hsv_layers.hsv_layer0.sat;
			hsv.v = hsv_layers.hsv_layer0.val;
		}
  	}
  }

  RGB rgb = hsv_to_rgb(hsv);
  
  for (uint8_t i = led_min; i <= led_max -1; i++) {
      if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
      }
  }
  return false;
}  