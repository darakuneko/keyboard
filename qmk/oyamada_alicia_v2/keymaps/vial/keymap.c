#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(

		KC_ESC,  KC_Q,    KC_P,    KC_LBRC, KC_RBRC,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_SCLN, KC_QUOT, KC_BSLS,
		KC_LSFT, KC_Z,    KC_DOT,  KC_SLSH, KC_RSFT,
		KC_LALT, KC_RALT,
		KC_W,    KC_E,    KC_R,    KC_T,
		KC_S,    KC_D,    KC_F,    KC_G,
		KC_X,    KC_C,    KC_V,    KC_B,
		KC_LGUI, LT(1, KC_DEL),LT(1, KC_SPC),
		KC_Y,    KC_U,    KC_I,    KC_O,
		KC_H,    KC_J,    KC_K,    KC_L,
		KC_N,    KC_M,    KC_COMM,
		 LT(1, KC_ENT),  LT(1, KC_BSPC),KC_RGUI
	),

	[1] = LAYOUT(

		KC_GRV,  KC_NO,   KC_NO,   KC_MINS, KC_EQL,
		KC_NO,   KC_1,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_6,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_2,    KC_3,    KC_4,    KC_5,
		KC_7,    KC_8,    KC_9,    KC_0,
		KC_NO,   KC_LSFT, MO(2),
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,
		KC_NO,   KC_NO,   KC_NO,
		MO(2),   KC_LSFT, KC_NO
	),

	[2] = LAYOUT(

		KC_NO,   KC_F1,   KC_F10,  KC_F11,  KC_F12,
		RGB_TOG, RGB_VAI, KC_NO,   KC_NO,   KC_NO,
		KC_NO,   RGB_VAD, KC_NO,   KC_NO,   KC_NO,
		EE_CLR,  QK_BOOT,
		KC_F2,   KC_F3,   KC_F4,   KC_F5,
		RGB_SAI, RGB_HUI, RGB_SPI, KC_NO,
		RGB_SAD, RGB_HUD, RGB_SPD, KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_F6,   KC_F7,   KC_F8,   KC_F9,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO
	),

	[3] = LAYOUT(

		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   KC_NO
	),

};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
	if(rgb_matrix_get_mode() == 1){
    	int current_layer = get_highest_layer(layer_state|default_layer_state);  
    	HSV hsv = {0, 255, rgb_matrix_get_val()};
    	if (current_layer == 1) {
      		hsv.h = 213; //MAGENTA
    	} else if (current_layer == 2)  {
      		hsv.h = 85; //GREEN
    	} else if (current_layer == 3)  {
      		hsv.h = 43; //YELLOW
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
