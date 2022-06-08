/* Copyright 2020 David Philip Barr <@davidphilipbarr>
 * Copyright 2021 @filterpaper
 * SPDX-License-Identifier: GPL-2.0+
 */

#include "quantum.h"

#define LAYOUT( \
    K05,   K06,   K07,   K08,   K09, \
    K10,   K11,   K12,   K13,   K14, \
    K15,   K16,   K17,   K18,   K19, \
    K20,   K21, \
    K00, K01, K02, K03, K04, \
    ELCCW, ELCW \
) \
{ \
    { K00, K01, K02, K03, K04,           ELCCW, ELCW, KC_NO, KC_NO, KC_NO, KC_NO, }, \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, K05,   K06,   K07,   K08,   K09,   K10, },  \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, K11,   K12,   K13,   K14,   K15,   K16, },   \
    { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, K17,   K18,   K19,   K20,   K21,   KC_NO }   \
}
