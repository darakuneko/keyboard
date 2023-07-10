/* Copyright 2020 sekigon-gonnoc
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
#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "report.h"

#define FINGER_MAX 3
#define GES_TIME_MS 200
#define TAP_TIME_MS 350

#define MIN_MOVE_FOR_GES 1000
#define MIN_MOVE_FOR_SWIPE_UPDATE 100
#define MIN_MOVE_FOR_PINCH_UPDATE 1500
#define MIN_MOVE_FOR_ROT_UPDATE 100

#define IQS_X_DIR (1)  // 1 or -1
#define IQS_Y_DIR (1)  // 1 or -1

#define FIRM_BLOCK_SIZE 64

typedef struct {
    uint16_t x;
    uint16_t y;
} point_t;

typedef struct {
    int16_t x;
    int16_t y;
} point_s_t;

typedef enum {
    FINGER_UP,
    FINGER_DOWN,
} finger_state_t;

typedef enum {
    GESTURE_NONE,
    GESTURE_SINGLE_TAP,
    GESTURE_MULTI_FINGER_TAP,
    GESTURE_PINCH_IDLE = 0x10,
    GESTURE_PINCH_IN,
    GESTURE_PINCH_OUT,
    GESTURE_PINCH_END  = GESTURE_PINCH_OUT,
    GESTURE_SWIPE_IDLE = 0x20,
    GESTURE_SWIPE_R,
    GESTURE_SWIPE_RU,
    GESTURE_SWIPE_U,
    GESTURE_SWIPE_UL,
    GESTURE_SWIPE_L,
    GESTURE_SWIPE_LD,
    GESTURE_SWIPE_D,
    GESTURE_SWIPE_DR,
    GESTURE_SWIPE_END = GESTURE_SWIPE_DR,
    GESTURE_ROT_IDLE  = 0x30,
    GESTURE_ROT_CW,
    GESTURE_ROT_CCW,
    GESTURE_ROT_END = GESTURE_ROT_IDLE,
} iqs5xx_gesture_t;

typedef struct {
    union {
        uint8_t bytes[4];
        point_t current;
    };
} iqs5xx_finger_t;

typedef struct {
    iqs5xx_finger_t fingers[FINGER_MAX];
    uint8_t         finger_cnt;
    uint8_t         mode;
} iqs5xx_data_t;

typedef struct {
    point_t        last;
    point_t        start;
    point_s_t      rel;
    point_t        last_gesture_point;
    point_s_t      frame_move;
    int16_t        dx, dy;
    uint32_t       t_start;
    uint32_t       t_tapped;
    bool           tapped;
    finger_state_t state;
} iqs5xx_finger_processed_t;

typedef struct {
    iqs5xx_finger_processed_t fingers[FINGER_MAX];
    uint8_t                   tap_cnt;
} iqs5xx_processed_data_t;

typedef struct {
    int16_t          dot;           // dot product of velocity
    int16_t          dot_rel;       // dot product of vector from entry point to current
    uint32_t         dist_sq_init;  // initial distance of fingres
    uint32_t         dist_sq;       // current distance of fingers
    iqs5xx_gesture_t gesture_state;
} iqs5xx_gesture_data_multi_t;

typedef struct {
    iqs5xx_gesture_data_multi_t multi;
} iqs5xx_gesture_data_t;

uint32_t ges_time;
uint32_t hold_drag_time;
bool hold_drag_mode;
bool is_tap_mode;
int hf_mode;
uint32_t drag_time;

bool is_layer_hf;
bool is_drag_mode;
bool press_ms_btn;
bool is_auto_trackpad_layer;
int auto_trackpad_layer;
bool change_auto_trackpad_layer;
float accel_speed;

// Application and bootloader I2C address in 7bit
#define IQS5xx_READ_ADDR 0x74
#define IQS5xx_BOOT_ADDR 0x34

uint16_t check_iqs5xx(void);
int      init_iqs5xx(void);
bool     read_iqs5xx(iqs5xx_data_t* const data);
bool     process_iqs5xx(iqs5xx_data_t const* const data, iqs5xx_processed_data_t* processed, report_mouse_t* const rep_mouse, iqs5xx_gesture_data_t* gesture);
void     pointing_device_set_button_iqs5xx(uint8_t btn);
void     pointing_device_clear_button_iqs5xx(uint8_t btn);
