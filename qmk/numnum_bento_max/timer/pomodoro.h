/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include QMK_KEYBOARD_H
#include "../config/device_config.h"
#include "../config/pomodoro_config.h"

// Pomodoro timer functions
void pomodoro_start(void);
void pomodoro_cancel(void);
void pomodoro_toggle(void);
void pomodoro_update(void);
void pomodoro_notify(void);
void update_pomodoro_active(void);