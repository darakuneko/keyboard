/* Copyright 2019 kakunpc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once

#include "quantum.h"

/* This a shortcut to help you visually see your layout.
 *
 * The first section contains all of the arguments representing the physical
 * layout of the board and position of the keys.
 *
 * The second converts the arguments into a two-dimensional array which
 * represents the switch matrix.
 */
#define LAYOUT( \
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, \
    k13, k14, k15, k16, k17, k18, k19, k20, k21, k22, k23, k24, \
    k25, k26, k27, k28, k29, k30, k31, k32, k33, k34, k35, k36, \
    k37, k38, k39, k40, k41, k42, k43, k44, k45, k46, k47, k48, k49, k50, \
    k51, k52, k53, k54, k55, k56, k57, k58, \
    k59, k62, k66, \
    k60, k63, k67, k70, \
    k61, k64, k68, k71, \
         k65, k69, \
    k72, ELCCW1, ELCW1,  k73, ELCCW2, ELCW2, k74, ELCCW3, ELCW3  \
) \
{ \
    { KC_NO, k10,   k19,   k28,   k37,   k46,   k55,   k64,   k72,   k74,    }, \
    { k01,   KC_NO, k20,   k29,   k38,   k47,   k56,   k65,   k73,   ELCCW1, }, \
    { k02,   k11,   KC_NO, k30,   k39,   k48,   k57,   k66,   KC_NO, ELCW1,  }, \
    { k03,   k12,   k21,   KC_NO, k40,   k49,   k58,   k67,   KC_NO, ELCCW2, }, \
    { k04,   k13,   k22,   k31,   KC_NO, k50,   k59,   k68,   KC_NO, ELCW2,  }, \
    { k05,   k14,   k23,   k32,   k41,   KC_NO, k60,   k69,   KC_NO, ELCCW3, }, \
    { k06,   k15,   k24,   k33,   k42,   k51,   KC_NO, k70,   KC_NO, ELCW3,  }, \
    { k07,   k16,   k25,   k34,   k43,   k52,   k61,   KC_NO, KC_NO, KC_NO,  }, \
    { k08,   k17,   k26,   k35,   k44,   k53,   k62,   k71,   KC_NO, KC_NO,  }, \
    { k09,   k18,   k27,   k36,   k45,   k54,   k63,   KC_NO, KC_NO, KC_NO   }, \
}
