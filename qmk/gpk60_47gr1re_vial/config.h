// Copyright 2023 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_ALL

#define I2C1_SDA_PIN        GP2
#define I2C1_SCL_PIN        GP3
#define I2C_DRIVER I2CD1
#define OLED_BRIGHTNESS 128
#define OLED_TIMEOUT 50000

#define VIAL_KEYBOARD_UID {0x3A, 0xA4, 0x2F, 0x88, 0x17, 0x6D, 0x4E, 0x51}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}
