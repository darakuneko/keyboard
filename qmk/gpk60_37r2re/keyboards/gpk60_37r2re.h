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
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11,  \
    k12, k13, k14, k15, k16, k17, k18, k19, k20, k21, k22,  \
    k23, k24, k25, k26, k27, k28, k29, k30, k31, k32, k33,   \
    k34, k35, k36, k37, k38, ELCCW1, ELCW1, k39, ELCCW2, ELCW2 \
) \
{ \
    { k01, k09, k17, k25, k33, }, \
    { k02, k10, k18, k26, k34, }, \
    { k03, k11, k19, k27, k35,  }, \
    { k04, k12, k20, k28, k36,  }, \
    { k05, k13, k21, k29, k37,  }, \
    { k06, k14, k22, k30, k38,  }, \
    { k07, k15, k23, k31, k39,  }, \
    { k08, k16, k24, k32, KC_NO, } , \
    { ELCCW1, ELCW1, ELCCW2, ELCW2, KC_NO } \
}
