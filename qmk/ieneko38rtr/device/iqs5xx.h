/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include "report.h"
#include "../config/trackpad_config.h"

#define FINGER_MAX 3
/* Default values defined in trackpad_config.h */
#define MIN_MOVE_THRESHOLD 1
#define MAX_HID_RANGE 32767
#define MISS_TAP_THRESHOLD 40000

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

#define I2C_RETRY_INTERVAL_MS 100
#define I2C_RETRY_COUNT 10
#define SCROLL_DIRECTION_THRESHOLD 128
#define SCROLL_INTERVAL_MS 10
#define SHORT_SCROLL_REPEAT 6
#define TAP_EDGE_THRESHOLD 128

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
    TAP_FINGER_FOUR
} iqs5xx_gesture_t;

typedef struct {
    uint8_t          finger_cnt;
    uint8_t          ges_evnet0;
    uint8_t          ges_evnet1;
    iqs5xx_xy_t      relative_xy;
    iqs5xx_xy_t      absolute_xy;
    uint16_t          touch_strenght1;
    iqs5xx_gesture_t gesture;
} iqs5xx_data_t;

typedef struct {
  uint32_t tap_time;
  uint32_t swipe_time;
  uint32_t gesture_time;
  uint32_t pinch_time;
  uint32_t scroll_time;
  uint32_t short_scroll_term;
  uint32_t drag_time;
} iqs_timer_t;

extern iqs_timer_t timer;
extern bool clear_buttons;
extern bool is_double_tap_detected;
extern bool is_trackpad_press;
extern bool is_tap_center_tap;
extern bool is_tap_left_tap;
extern bool is_tap_right_tap;

void init_iqs5xx(void);
bool read_iqs5xx(iqs5xx_data_t* const data);
void process_iqs5xx(iqs5xx_data_t* const data, report_mouse_t* const rep_mouse);
void set_iqs5xx_zoom_distance(uint16_t distance);