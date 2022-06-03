#pragma once

#include "config_common.h"

// USB descriptor
#define VENDOR_ID 0x4649
#define PRODUCT_ID 0x0721
#define DEVICE_VER 0x0001
#define MANUFACTURER illness072
#define PRODUCT elephant42
#define DESCRIPTION very very cute keyboard

// matrix
#define DIODE_DIRECTION COL2ROW
#define MATRIX_ROWS 8
#define MATRIX_COLS 6
#define MATRIX_ROW_PINS { D4, C6, D7, E6 }
#define MATRIX_COL_PINS { F4, F5, F6, F7, B1, B3 }
#define UNUSED_PINS

// split (Serial)
#define SOFT_SERIAL_PIN D2
#define SERIAL_USE_MULTI_TRANSACTION

// RGB LED/
#ifdef RGBLIGHT_ENABLE
#define RGB_DI_PIN D3
#define RGBLED_NUM 54    // Number of LEDs. backlight x42 + underglow x12
#define RGBLED_SPLIT { 27, 27 }
#define RGBLIGHT_LIMIT_VAL 120 /* The maximum brightness level */
#define RGBLIGHT_HUE_STEP 10
#define RGBLIGHT_SAT_STEP 17
#define RGBLIGHT_VAL_STEP 17
#define RGBLIGHT_ANIMATIONS
#endif

// MISC
#define DEBOUNCE 5
#define LOCKING_SUPPORT_ENABLE
#define LOCKING_RESYNC_ENABLE
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
