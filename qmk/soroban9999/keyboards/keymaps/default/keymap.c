/* Copyright 2020 illness072
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
enum layer_names {
    _BASE,
    _Fn1,
    _Fn2,
    _Fn3,
    _Fn4
};

#define _ROTALY_L 3
#define _ROTALY_R 7

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC__VOLDOWN,KC_BRMD,LCMD(KC_Z)   ,LCMD(KC_MINS),
        KC__VOLUP  ,KC_BRMU,LCMD(S(KC_Z)),LCMD(KC_PLUS),
        KC_MUTE, XXXXXXX, XXXXXXX, LCMD(KC_0),
        KC_P7  , KC_P8  , KC_P9  , LT(_Fn4, KC_PSLS),
        KC_P4  , KC_P5  , KC_P6  , LT(_Fn3, KC_PAST),
        KC_P1  , KC_P2  , KC_P3  , LT(_Fn2, KC_PMNS),
        KC_PEQL, KC_P0  , KC_PDOT, LT(_Fn1, KC_PPLS)
    ),
    [_Fn1] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        KC_ESC , _______, _______, XXXXXXX,
        KC_DEL , KC_BSPC, _______, XXXXXXX,
        KC_TAB , _______, KC_COLN, XXXXXXX,
        KC_PENT, KC_SPC,  KC_COMM, XXXXXXX
    ),
    [_Fn2] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, KC_LABK, KC_RABK, XXXXXXX,
        _______, KC_LCBR, KC_RCBR, XXXXXXX,
        _______, KC_LBRC, KC_RBRC, XXXXXXX,
        _______, KC_LPRN, KC_RPRN, XXXXXXX
    ),
    [_Fn3] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        KC_HOME, KC_UP  , KC_PGUP, XXXXXXX,
        KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX,
        KC_END , KC_DOWN, KC_PGDN, XXXXXXX,
        _______, KC_INS , _______, XXXXXXX
    ),
    [_Fn4] = LAYOUT(
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        _______, _______, _______, _______,
        RGB_TOG, RGB_MOD, RGB_RMOD, XXXXXXX,
        KC_D   , KC_E   , KC_F   , XXXXXXX,
        KC_A   , KC_B   , KC_C   , XXXXXXX,
        _______, _______, _______, XXXXXXX
    )
};


keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 3, .col = 0},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 7, .col = 0},
    .pressed = false
}; 

keyevent_t encoder2_ccw = {
    .key = (keypos_t){.row = 3, .col = 1},
    .pressed = false
};

keyevent_t encoder2_cw = {
    .key = (keypos_t){.row = 7, .col = 1},
    .pressed = false
}; 

keyevent_t encoder3_ccw = {
    .key = (keypos_t){.row = 3, .col = 2},
    .pressed = false
};

keyevent_t encoder3_cw = {
    .key = (keypos_t){.row = 7, .col = 2},
    .pressed = false
}; 

keyevent_t encoder4_ccw = {
    .key = (keypos_t){.row = 3, .col = 3},
    .pressed = false
};

keyevent_t encoder4_cw = {
    .key = (keypos_t){.row = 7, .col = 3},
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

    if (IS_PRESSED(encoder2_ccw)) {
        encoder2_ccw.pressed = false;
        encoder2_ccw.time = (timer_read() | 1);
        action_exec(encoder2_ccw);
    }

    if (IS_PRESSED(encoder2_cw)) {
        encoder2_cw.pressed = false;
        encoder2_cw.time = (timer_read() | 1);
        action_exec(encoder2_cw);
    }
    
    if (IS_PRESSED(encoder3_ccw)) {
        encoder3_ccw.pressed = false;
        encoder3_ccw.time = (timer_read() | 1);
        action_exec(encoder3_ccw);
    }

    if (IS_PRESSED(encoder3_cw)) {
        encoder3_cw.pressed = false;
        encoder3_cw.time = (timer_read() | 1);
        action_exec(encoder3_cw);
    }

    if (IS_PRESSED(encoder4_ccw)) {
        encoder4_ccw.pressed = false;
        encoder4_ccw.time = (timer_read() | 1);
        action_exec(encoder4_ccw);
    }

    if (IS_PRESSED(encoder4_cw)) {
        encoder4_cw.pressed = false;
        encoder4_cw.time = (timer_read() | 1);
        action_exec(encoder4_cw);
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    } else if (index == 1) {
        if (clockwise) {
            encoder2_cw.pressed = true; 
            encoder2_cw.time = (timer_read() | 1);
            action_exec(encoder2_cw);
        } else {
            encoder2_ccw.pressed = true;
            encoder2_ccw.time = (timer_read() | 1);
            action_exec(encoder2_ccw);
        }
    } else if (index == 2) {
        if (clockwise) {
            encoder3_cw.pressed = true; 
            encoder3_cw.time = (timer_read() | 1);
            action_exec(encoder3_cw);
        } else {
            encoder3_ccw.pressed = true;
            encoder3_ccw.time = (timer_read() | 1);
            action_exec(encoder3_ccw);
        }
    }else if (index == 3) {
        if (clockwise) {
            encoder4_cw.pressed = true; 
            encoder4_cw.time = (timer_read() | 1);
            action_exec(encoder4_cw);
        } else {
            encoder4_ccw.pressed = true;
            encoder4_ccw.time = (timer_read() | 1);
            action_exec(encoder4_ccw);
        }
    }

    return true;
}
