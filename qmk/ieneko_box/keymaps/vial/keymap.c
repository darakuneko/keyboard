/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "device/mpr121.h"
#include "i2c_master.h"
#include "qmk_settings.h"

keypos_t dip_key_push = (keypos_t){.row = 5, .col = 0};
keypos_t dip_key_release = (keypos_t){.row = 5, .col = 1};

keypos_t mpr121_1_ele0_group = (keypos_t){.row = 5, .col = 2};
keypos_t mpr121_1_ele1_group = (keypos_t){.row = 5, .col = 3};
keypos_t mpr121_1_ele2_group = (keypos_t){.row = 5, .col = 4};
keypos_t mpr121_1_slider_up = (keypos_t){.row = 5, .col = 5};
keypos_t mpr121_1_slider_down = (keypos_t){.row = 5, .col = 6};

keypos_t mpr121_2_ele0_group = (keypos_t){.row = 6, .col = 0};
keypos_t mpr121_2_ele1_group = (keypos_t){.row = 6, .col = 1};
keypos_t mpr121_2_ele2_group = (keypos_t){.row = 6, .col = 2};
keypos_t mpr121_2_slider_up = (keypos_t){.row = 6, .col = 3};
keypos_t mpr121_2_slider_down = (keypos_t){.row = 6, .col = 4};

// MPR121 scan interval (ms)
#define MPR121_SCAN_INTERVAL_MS 48

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	[0] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
		MO(2),   KC_LGUI, KC_LALT, LT(1, KC_SPC),LT(1, KC_BSPC),KC_RALT, KC_RGUI,   MO(2),
		KC_1,    KC_2,    KC_3,    KC_4,    KC_5,	    KC_6,      KC_7,
		DF(4),   DF(0),   KC_MPRV,    KC_MPLY,    KC_MNXT,      KC_VOLD, KC_VOLU,
		KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_VOLD, KC_VOLU
	),

	[1] = LAYOUT(
		KC_GRV,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS,  KC_EQL,
		KC_LCTL, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL,
		KC_LSFT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LBRC, KC_RBRC,  KC_SCLN, KC_QUOT, KC_SLSH,
		XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),

	[2] = LAYOUT(
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),

	[3] = LAYOUT(
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),

	[4] = LAYOUT(
		KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
		MT(MOD_LCTL,KC_TAB),KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_ENT,
		KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_RSFT,
		MO(2),   KC_LGUI, KC_LALT, LT(1, KC_SPC),LT(1, KC_BSPC),KC_RALT, KC_RGUI,   MO(2),
		KC_1,    KC_2,    KC_3,    KC_4,    KC_5,	    KC_6,      KC_7,
		DF(4),   DF(0),   KC_MPRV,    KC_MPLY,    KC_MNXT,      KC_VOLD, KC_VOLU,
		KC_MPRV,    KC_MPLY,    KC_MNXT,    KC_VOLD, KC_VOLU
	),

	[5] = LAYOUT(
		KC_GRV,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_MINS,  KC_EQL,
		KC_LCTL, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_DEL,
		KC_LSFT, KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LBRC, KC_RBRC,  KC_SCLN, KC_QUOT, KC_SLSH,
		XXXXXXX, XXXXXXX, XXXXXXX, KC_LSFT, KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),

	[6] = LAYOUT(
		KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),

	[7] = LAYOUT(
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, 
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
		XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
	),
};

void gesture_press_key_pressed(keypos_t k, bool pressed) {
    keyevent_t k_event = { .key = k, .type = KEY_EVENT };
    k_event.pressed = pressed;
    k_event.time = (timer_read() | 1);
    action_exec(k_event);   
}

void gesture_press_key(keypos_t k) {
    gesture_press_key_pressed(k, true);
    gesture_press_key_pressed(k, false);
}

void keyboard_post_init_user(void) {
	wait_ms(1000); // Wait for things to settle
	mpr121_init();
}

static uint16_t i2c_scan_term = 0;

void matrix_scan_user(void) {
	// Reduce MPR121 scan frequency to prevent LED flickering
	if (timer_elapsed(i2c_scan_term) < MPR121_SCAN_INTERVAL_MS) {
        return;
    }

	mpr121_scan();

	// Handle MPR121 #1 touch events
	// - Tap (short touch): Triggered when finger is released
	// - Long press: Triggered after 1000ms hold, repeats every QS.tapping_term
	// - Slider: Triggered when sliding between electrodes, repeats every 80ms
	//   (Slider events respect tap timing to prevent rapid alternation)
	if (mpr121_data_1.ele0_touched) {
		gesture_press_key(mpr121_1_ele2_group);
	}
	if (mpr121_data_1.ele1_touched) {
		gesture_press_key(mpr121_1_ele2_group);
	}
	if (mpr121_data_1.ele2_touched) {
		gesture_press_key(mpr121_1_ele1_group);
	}
	if (mpr121_data_1.ele3_touched) {
		gesture_press_key(mpr121_1_ele1_group);
	}
	if (mpr121_data_1.ele4_touched) {
		gesture_press_key(mpr121_1_ele1_group);
	}
	if (mpr121_data_1.ele5_touched) {
		gesture_press_key(mpr121_1_ele1_group);
	}
	if (mpr121_data_1.ele6_touched) {
		gesture_press_key(mpr121_1_ele0_group);
	}
	if (mpr121_data_1.ele7_touched) {
		gesture_press_key(mpr121_1_ele0_group);
	}

	// Handle MPR121 #1 slider events (one-shot)
	if (mpr121_data_1.slider_up) {
		gesture_press_key(mpr121_1_slider_down);
	}
	if (mpr121_data_1.slider_down) {
		gesture_press_key(mpr121_1_slider_up);
	}

	// Handle MPR121 #2 touch events
	if (mpr121_data_2.ele0_touched) {
		gesture_press_key(mpr121_2_ele2_group);
	}
	if (mpr121_data_2.ele1_touched) {
		gesture_press_key(mpr121_2_ele2_group);
	}
	if (mpr121_data_2.ele2_touched) {
		gesture_press_key(mpr121_2_ele1_group);
	}
	if (mpr121_data_2.ele3_touched) {
		gesture_press_key(mpr121_2_ele1_group);
	}
	if (mpr121_data_2.ele4_touched) {
		gesture_press_key(mpr121_2_ele1_group);
	}
	if (mpr121_data_2.ele5_touched) {
		gesture_press_key(mpr121_2_ele1_group);
	}
	if (mpr121_data_2.ele6_touched) {
		gesture_press_key(mpr121_2_ele0_group);
	}
	if (mpr121_data_2.ele7_touched) {
		gesture_press_key(mpr121_2_ele0_group);
	}

	// Handle MPR121 #2 slider events (one-shot)
	if (mpr121_data_2.slider_up) {
		gesture_press_key(mpr121_2_slider_down);
	}
	if (mpr121_data_2.slider_down) {
		gesture_press_key(mpr121_2_slider_up);
	}
	i2c_scan_term = timer_read();
}

bool dip_switch_update_user(uint8_t index, bool active) {
	uprintf("dip_switch_update_user %d is %s\n", index, active ? "ON" : "OFF");
    switch (index) {
        case 0:
            if(active) {
				gesture_press_key(dip_key_push);
			} else {
				gesture_press_key(dip_key_release);
			 }
            break;
    }
    return true;
}

bool rgb_matrix_indicators_user(void){
	return true;
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	int rgb_matrix_mode = rgb_matrix_get_mode();
  	if(rgb_matrix_mode != 1){ return false; }

    int curr_layer = get_highest_layer(layer_state | default_layer_state);
    HSV hsv = {0, 255, rgb_matrix_get_val()};

    switch (curr_layer) {
        case 1:
            hsv.h = 43;  // YELLOW
            break;
        case 2:
            hsv.h = 11;  // CORAL
            break;
        case 3:
            hsv.h = 191; // PURPLE
            break;
        case 4:
            hsv.h = 85;  // GREEN
            break;
        case 5:
            hsv.h = 0;   // RED
            break;
        case 6:
            hsv.h = 170; // BLUE
            break;
        case 7:
            hsv.h = 213; // MAGENTA
            break;
        default:
            hsv.h = 128; // CYAN (default)
            break;
    }

    RGB rgb = hsv_to_rgb(hsv);

    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
}
