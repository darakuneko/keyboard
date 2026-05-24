// Copyright 2026
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID {0x32, 0x01, 0x4C, 0x51, 0xC5, 0x0F, 0xD8, 0x2E}

#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {0, 1}

/* RS16211 1P8T Rotary Switch — 8 positions on GP1-GP8 */
#define DIP_SWITCH_PINS { GP1, GP2, GP3, GP4, GP5, GP6, GP7, GP8 }

/* M-N11S1E Toggle Switch — wired into key matrix at [4,3].
 * Handled manually in matrix_scan_user() because DIP_SWITCH_MATRIX_GRID
 * positions are excluded from NUM_DIP_SWITCHES when DIP_SWITCH_MAP_ENABLE
 * is active, making the map entry silently unused.
 */
#define TOGGLE_SWITCH_ROW 4
#define TOGGLE_SWITCH_COL 3
