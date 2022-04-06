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
    k01, k02, k03, k04, k05, k06, k07, k08, k09, k10, k11, k12, k13, k14, k15, k16, \
    k17, k18, k19, k20, k21, k22, k23, k24, k25, k26, k27, k28, k29, k30, k31, k32, \
    k33, k34, k35, k36, k37, k38, k39, k40, k41, k42, k43, k44, k45, k46, k47, k48, \
    k49, k50, k51, k52, k53, k54, k55, k56, k57, k58, k59, k60, k61, k62, k63, k64, \
    k65, k66, k67, k68, k69, k70, k71, k72, k73, k74, k75, k76, k77, k78, k79, k80,   \
    k81, k82, k83, k84,   k85,      k86,    k87, k88, k89, k90, k91, k92, k93, k94,   \
    k95, ELCCW1, ELCW1,  k96, ELCCW2, ELCW2, k97, ELCCW3, ELCW3  \
) \
{ \
    { KC_NO, k11,   k21,   k31,   k41,   k51,   k61,   k71,   k81,   k91,   k94, }, \
    { k01,   KC_NO, k22,   k32,   k42,   k52,   k62,   k72,   k82,   k92,   k95, }, \
    { k02,   k12,   KC_NO, k33,   k43,   k53,   k63,   k73,   k83,   KC_NO, k96, }, \
    { k03,   k13,   k23,   KC_NO, k44,   k54,   k64,   k74,   k84,   KC_NO, k97, }, \
    { k04,   k14,   k24,   k34,   KC_NO, k55,   k65,   k75,   k85,   KC_NO, ELCCW1, }, \
    { k05,   k15,   k25,   k35,   k45,   KC_NO, k66,   k76,   k86,   KC_NO, ELCW1, }, \
    { k06,   k16,   k26,   k36,   k46,   k56,   KC_NO, k77,   k87,   KC_NO, ELCCW2, }, \
    { k07,   k17,   k27,   k37,   k47,   k57,   k67,   KC_NO, k88,   KC_NO, ELCW2, }, \
    { k08,   k18,   k28,   k38,   k48,   k58,   k68,   k78,   KC_NO, KC_NO, ELCCW3, }, \
    { k09,   k19,   k29,   k39,   k49,   k59,   k69,   k79,   k89,   KC_NO, ELCW3, }, \
    { k10,   k20,   k30,   k40,   k50,   k60,   k70,   k80,   k90,   k93,   KC_NO  } \
}
