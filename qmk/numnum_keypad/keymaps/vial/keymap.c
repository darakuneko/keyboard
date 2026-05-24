/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "devices/tm1637.h"

enum {
  MY_00 = QK_KB_0
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_PSLS, KC_PAST, KC_BSPC,
        KC_P7,   KC_P8,   KC_P9,   KC_PMNS,
        KC_P4,   KC_P5,   KC_P6,   KC_PPLS,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,
        KC_P0,   MY_00,   KC_PDOT, KC_NO    /* [4,3] = DIP switch (toggle) */
    ),

    [1] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [2] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [3] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [4] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [5] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [6] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [7] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    ),

    [8] = LAYOUT(
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO, KC_NO, KC_NO, KC_NO
    )
};

/* --- Display & state management --- */

/* DIP bit → layer mapping (rotary switch wired in reverse within each bank of 4) */
static const uint8_t dip_to_layer[8] = {4, 3, 2, 1, 8, 7, 6, 5};

static bool     numlock_active     = false;
static uint8_t  saved_dip_layer    = 0;
static uint32_t last_activity      = 0;
static bool     display_sleeping   = false;
static bool     toggle_switch_state = false;

#define DISPLAY_TIMEOUT_MS 180000

static void update_display(layer_state_t combined_state) {
    tm1637_clear();
    wait_ms(40);
    if (numlock_active) {
        tm1637_display_raw(0, 0x80);
        tm1637_display_raw(1, 0x80);
    } else {
        uint8_t layer = get_highest_layer(combined_state);
        tm1637_display_number(layer);
    }
}

static void render_display(void) {
    update_display(layer_state | default_layer_state);
}

static void apply_numlock_state(bool active) {
    numlock_active = active;
    if (active) {
        layer_clear();
        default_layer_set(1);  /* layer 0 */
    } else {
        default_layer_set(1 << saved_dip_layer);
    }
    last_activity = timer_read32();
    /* Wake display if sleeping (toggle switch bypasses process_record_user) */
    if (display_sleeping) {
        display_sleeping = false;
        tm1637_set_brightness(TM1637_BRIGHTNESS_DEFAULT, true);
        render_display();
    }
}

/* --- RS16211 1P8T Rotary Switch (GP1-GP8) ---
 * Reads the bitmask of all 8 positions and sets the default layer
 * to match the active position. Avoids event-ordering issues that
 * dip_switch_map + DF() can have during contact transitions.
 */
bool dip_switch_update_mask_user(uint32_t state) {
    for (uint8_t i = 0; i < 8; i++) {
        if (state & (1 << i)) {
            saved_dip_layer = dip_to_layer[i];
            if (!numlock_active) {
                default_layer_set(1 << saved_dip_layer);
            }
            return true;
        }
    }
    /* No position active (contact transition) - keep current layer */
    return true;
}

/* --- M-N11S1E Toggle Switch [TOGGLE_SWITCH_ROW, TOGGLE_SWITCH_COL] --- */

void matrix_scan_user(void) {
    bool current = matrix_is_on(TOGGLE_SWITCH_ROW, TOGGLE_SWITCH_COL);
    if (current != toggle_switch_state) {
        toggle_switch_state = current;
        apply_numlock_state(current);
    }

    /* Auto-off: turn display off after 3 minutes of inactivity */
    if (!display_sleeping && timer_elapsed32(last_activity) > DISPLAY_TIMEOUT_MS) {
        tm1637_set_brightness(TM1637_BRIGHTNESS_DEFAULT, false);
        display_sleeping = true;
    }
}

void keyboard_post_init_user(void) {
    tm1637_init();
    last_activity = timer_read32();
    toggle_switch_state = matrix_is_on(TOGGLE_SWITCH_ROW, TOGGLE_SWITCH_COL);
    if (toggle_switch_state) {
        apply_numlock_state(true);
    } else {
        /* DIP callback hasn't fired yet; just show layer 0 for now */
        render_display();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    last_activity = timer_read32();
    if (display_sleeping) {
        display_sleeping = false;
        tm1637_set_brightness(TM1637_BRIGHTNESS_DEFAULT, true);
        render_display();
    }
    switch (keycode) {  
        case MY_00:
            if (record->event.pressed) {
                SEND_STRING("00");
            }
            return false;
        default:
            return true;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (!display_sleeping) {
        update_display(state | default_layer_state);
    }
    return state;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    if (!display_sleeping) {
        update_display(layer_state | state);
    }
    return state;
}

/* --- Per-layer RGB indicator color (Solid Color mode only) --- */
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (rgb_matrix_get_mode() != RGB_MATRIX_SOLID_COLOR) {
        return false;
    }

    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    HSV hsv = {0, 255, rgb_matrix_get_val()};

    switch (layer) {
        case 1:  hsv.h = 191; break; /* Purple */
        case 2:  hsv.h =  85; break; /* Green  */
        case 3:  hsv.h =  43; break; /* Yellow */
        case 4:  hsv.h =   0; break; /* Red    */
        case 5:  hsv.h =  21; break; /* Orange */
        case 6:  hsv.h = 170; break; /* Blue   */
        case 7:  hsv.h = 213; break; /* Pink   */
        case 8:  hsv.h =  32; break; /* Gold   */
        default: hsv.h = 128; break; /* Cyan   */
    }

    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
}
