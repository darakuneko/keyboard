/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "report.h"
#include "../config/trackpad_config.h"

#define FINGER_MAX 3
/* Default values defined in trackpad_config.h */
#define MIN_MOVE_THRESHOLD 1
#define MAX_SPEED 32767

#define IQS5xx_COM_END_REG 0xEEEE
#define IQS5xx_GESTURE_EVENT0 0x000D
#define IQS5xx_GESTURE_EVENT1 0x000E
#define IQS5xx_FINGER_NUM 0x0011
#define IQS5xx_RELATIVE_XY 0x0012
#define IQS5xx_ABSOLUTE_XY 0x0016
#define IQS5xx_TOUCH_STRENGTH_FINGER1 0x001A
#define IQS5xx_ATI_TARGET 0x056D
#define IQS5xx_FINGER_SPLIT 0x066B
#define IQS5xx_HOLD_TIME 0x06BD
#define IQS5xx_ZOOM 0x06CB
#define IQS5xx_DEFAULT_READ 0x0675
#define IQS5xx_DEVICE_ADDR 0x74

typedef struct {
    union {
        uint8_t bytes[4];
    };
} iqs5xx_xy_t;

typedef enum {
    GESTURE_SWIPE_L = 0x10,    
    GESTURE_SWIPE_R,
    GESTURE_SWIPE_U,
    GESTURE_SWIPE_D,
    GESTURE_PINCH_OUT,
    GESTURE_PINCH_IN,
    TAP_FINGER_ONE_LEFT,
    TAP_FINGER_ONE_CENTER,
    TAP_FINGER_ONE_RIGHT,
    TAP_FINGER_TWO,
    TAP_FINGER_THREE,
} iqs5xx_gesture_t;

typedef struct {
    uint8_t          finger_cnt;
    uint8_t          ges_evnet0;
    uint8_t          ges_evnet1;
    iqs5xx_xy_t      relative_xy;
    iqs5xx_xy_t      absolute_xy;
    uint8_t          touch_strenght1;
    iqs5xx_gesture_t gesture;
} iqs5xx_data_t;

extern bool clear_buttons;

void init_iqs5xx(void);
bool read_iqs5xx(iqs5xx_data_t* const data);
void process_iqs5xx(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse);
void set_iqs5xx_zoom_distance(uint16_t distance);