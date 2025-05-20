/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include QMK_KEYBOARD_H

// Pomodoro states
typedef enum {
    POMODORO_IDLE,       // Stopped
    POMODORO_WORK,       // Working
    POMODORO_BREAK,      // Break
    POMODORO_LONG_BREAK  // Long break
} pomodoro_phase_t;

typedef struct {
    unsigned int work_time : 8;
    unsigned int break_time : 8;
    unsigned int long_break_time : 8;
    unsigned int work_interval : 4;
    unsigned int work_hf_pattern : 8; 
    unsigned int break_hf_pattern : 8;
    bool notify_haptic_enable : 1;
    bool continuous_mode : 1;
    unsigned int pomodoro_cycle : 4;
} pomodoro_config_t;

extern pomodoro_config_t pomodoro_config;

// Pomodoro state variables
extern pomodoro_phase_t phase;
extern uint16_t minutes;
extern uint16_t seconds;
extern uint8_t current_work_interval;
extern uint8_t current_pomodoro_cycle;
extern bool timer_active;
extern bool waiting_for_break_touch;

void init_pomodoro_config(pomodoro_config_t *pomodoro_config);
void update_pomodoro_config(pomodoro_config_t pomodoro_config);
void set_pomodoro_config(pomodoro_config_t pomodoro_config);
void send_pomodoro_config(const pomodoro_config_t *config);
void send_pomodoro_active_status(const pomodoro_config_t *config);
void receive_pomodoro_config(uint8_t *data);