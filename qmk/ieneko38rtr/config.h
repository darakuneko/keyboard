// Copyright 2023 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID {0xCB, 0x61, 0xD2, 0x8E, 0x12, 0x3F, 0x40, 0xF6}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET

#define I2C1_SDA_PIN        GP2
#define I2C1_SCL_PIN        GP3

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

#define EECONFIG_USER_DATA_SIZE 48

#define MOUSE_EXTENDED_REPORT