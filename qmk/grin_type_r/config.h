// Copyright 2022 GRINkeebs (@policium)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#pragma once

#define VIAL_KEYBOARD_UID {0xDB, 0x64, 0x72, 0x8A, 0x12, 0x3F, 0x40, 0xF6}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

#define I2C1_SDA_PIN        GP26
#define I2C1_SCL_PIN        GP27

/* Haptic feedback */
#define FB_ERM_LRA 0
#define FB_BRAKEFACTOR 3 // For 1x:0, 2x:1, 3x:2, 4x:3, 6x:4, 8x:5, 16x:6, Disable Braking:7
#define FB_LOOPGAIN 1 // For  Low:0, Medium:1, High:2, Very High:3
/* Motor settings */
#define RATED_VOLTAGE 3
#define V_PEAK 5
#define HAPTIC_FEEDBACK_DEFAULT 1
#define DRV_MODE_DEFAULT 14
#define DRV_GREETING       alert_750ms

#define AUDIO_PIN GP28
#define AUDIO_PWM_DRIVER PWMD6
#define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#define AUDIO_CLICKY
#define AUDIO_CLICKY_FREQ_RANDOMNESS 1.0f
#define DEFAULT_LAYER_SONGS { SONG(PLANCK_SOUND) }