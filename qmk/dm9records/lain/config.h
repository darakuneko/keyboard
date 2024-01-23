// Copyright 2022 Takuya Urakawa @hsgw (dm9records.com, 5z6p.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

/* LED Settings */
#define LED_NUM 3
#define LED_PINS \
    { B6, B5, B4 }

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

#define VIAL_KEYBOARD_UID {0xA4, 0x46, 0x33, 0x2C, 0x40, 0x79, 0x04, 0xD4}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}
