// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

// Same order as customKeycodes in keymaps/vial/vial.json.
// Removed 2026-11-22: BT_1..BT_4 (four BLE hosts: flash erases on the nRF at every switch, a
// current burst on the coin cell), BT_PAIR and BT_CLR (pairings are erased over USB:
// tools/hy0020_update.py --erase-pairings) and OUT_AUTO / OUT_USB / OUT_BT (the output is always
// automatic: USB when a host is on the cable, else BLE).
enum trident40_keycodes {
    BAT_LVL = QK_KB_0,
    BT_UPDT,
};
