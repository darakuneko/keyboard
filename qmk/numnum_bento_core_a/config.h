// Copyright 2023 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_STARTUP_MODE RGB_MATRIX_CYCLE_ALL

#define I2C1_SDA_PIN        GP2
#define I2C1_SCL_PIN        GP3

#define VIAL_KEYBOARD_UID {0xEA, 0xFA, 0x04, 0x88, 0x9B, 0x23, 0xD8, 0xFB}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {5, 6}

/* Haptic feedback */
#define DRV2605L_FB_ERM_LRA 0
#define DRV2605L_FB_BRAKEFACTOR 3 // For 1x:0, 2x:1, 3x:2, 4x:3, 6x:4, 8x:5, 16x:6, Disable Braking:7
#define DRV2605L_FB_LOOPGAIN 1 // For  Low:0, Medium:1, High:2, Very High:3
/* Motor settings */
#define DRV2605L_RATED_VOLTAGE 3
#define DRV2605L_V_PEAK 5
#define DRV2605L_V_RMS 2.0
#define DRV2605L_F_LRA 205 /* resonance freq */

#define DRV2605L_DEFAULT_MODE 0
#define DRV2605L_GREETING DRV2605L_EFFECT_750_MS_ALERT_100

#define EECONFIG_USER_DATA_SIZE 6