/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "pomodoro_config.h"
#include "../timer/pomodoro.h"
#include "drivers/haptic/drv2605l.h"

// Pomodoro configuration structure
pomodoro_config_t pomodoro_config;

// Pomodoro state variables
pomodoro_phase_t phase = POMODORO_IDLE;
uint16_t minutes = 0;
uint16_t seconds = 0;
uint8_t current_work_interval = 0;
uint8_t current_pomodoro_cycle = 0;
bool timer_active = false;
bool waiting_for_break_touch = false;
bool init_pomodoro_config_flag = true;

void init_pomodoro_config(pomodoro_config_t *pomodoro_config) {
  pomodoro_config->work_time = 25;
  pomodoro_config->break_time = 5;
  pomodoro_config->long_break_time = 15;
  pomodoro_config->work_interval = 4;
  pomodoro_config->work_hf_pattern = 119;
  pomodoro_config->break_hf_pattern = 16;
  pomodoro_config->notify_haptic_enable = true;
  pomodoro_config->continuous_mode = false;
  pomodoro_config->pomodoro_cycle = 3;
  update_pomodoro_config(*pomodoro_config);
}

// Apply pomodoro configuration to the timer variables
void update_pomodoro_config(pomodoro_config_t pomodoro_config_param) {
  if(!init_pomodoro_config_flag && pomodoro_config.work_hf_pattern != pomodoro_config_param.work_hf_pattern){
    drv2605l_pulse(pomodoro_config_param.work_hf_pattern);
  }
  if(!init_pomodoro_config_flag && pomodoro_config.break_hf_pattern != pomodoro_config_param.break_hf_pattern){
    drv2605l_pulse(pomodoro_config_param.break_hf_pattern);
  }
  
  init_pomodoro_config_flag = false; // Set flag to false after first initialization
  pomodoro_config = pomodoro_config_param;  
  device_config.pomodoro_config = pomodoro_config_param;
  save_device_config(device_config);
} 

void set_pomodoro_config(pomodoro_config_t pomodoro_config) {
  update_pomodoro_config(pomodoro_config);
  
  phase = POMODORO_IDLE;
  minutes = 0;
  seconds = 0;
  current_work_interval = 0;
  timer_active = false;
  waiting_for_break_touch = false;
}

void send_pomodoro_config(const pomodoro_config_t *config) {
  uint8_t data[32] = {0};
  
  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_get_value;
  data[2] = id_pomodoro_get_value;
  
  data[3] = config->work_time;
  data[4] = config->break_time;
  data[5] = config->long_break_time;
  data[6] = config->work_interval;
  data[7] = config->work_hf_pattern;
  data[8] = config->break_hf_pattern;  
  data[9] = timer_active << 7 | 
            config->notify_haptic_enable << 6 | 
            config->continuous_mode << 5 |
            (phase & 0b00000011);
  data[10] = config->pomodoro_cycle; 

  uprintf("work_time: %d\n", config->work_time);
  uprintf("break_time: %d\n", config->break_time);
  uprintf("long_break_time: %d\n", config->long_break_time);
  uprintf("work_interval: %d\n", config->work_interval);
  uprintf("pomodoro_cycle: %d\n", config->pomodoro_cycle);
  uprintf("work_hf_pattern: %d\n", config->work_hf_pattern);
  uprintf("break_hf_pattern: %d\n", config->break_hf_pattern);
  uprintf("timer_active: %d\n", timer_active);
  uprintf("notify_haptic_enable: %d\n", config->notify_haptic_enable);
  uprintf("continuous_mode: %d\n", config->continuous_mode);
  uprintf("phase: %d\n", phase);

  uprintf("\n\n");
  raw_hid_send(data, sizeof(data));
}

void send_pomodoro_active_status(const pomodoro_config_t *config) {
  uint8_t data[32] = {0};
  
  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_get_value;
  data[2] = id_pomodoro_active_get_value;
  
  data[3] = timer_active << 7|
            (phase & 0b00000011);
  data[4] = minutes;
  data[5] = seconds;
  data[6] = current_work_interval;
  data[7] = current_pomodoro_cycle;

  uprintf("pomodoro_timer_active: %d\n", timer_active);
  uprintf("pomodoro_notify_haptic_enable: %d\n", config->notify_haptic_enable);
  uprintf("pomodoro_continuous_mode: %d\n", config->continuous_mode);
  uprintf("pomodoro_active: %d\n", phase);
  uprintf("current_pomodoro_cycle: %d\n", current_pomodoro_cycle);
  uprintf("pomodoro_cycle: %d\n", config->pomodoro_cycle);
  uprintf("minutes: %d\n", minutes);
  uprintf("seconds: %d\n", seconds);
  uprintf("current_work_interval: %d\n", current_work_interval);
  uprintf("\n\n");

  raw_hid_send(data, sizeof(data));
}

void receive_pomodoro_config(uint8_t *data) {
  pomodoro_config_t temp_config;
  temp_config.work_time = data[0];
  temp_config.break_time = data[1];
  temp_config.long_break_time = data[2];
  temp_config.work_interval = data[3] & 0b00001111;
  temp_config.work_hf_pattern = data[4];
  temp_config.break_hf_pattern = data[5];
  temp_config.notify_haptic_enable = (data[6] & 0b01000000) >> 6;
  temp_config.continuous_mode = (data[6] & 0b00100000) >> 5;
  temp_config.pomodoro_cycle = data[7] & 0b00001111;
  
  uprintf("pomodoro_config.work_time: %d\n", temp_config.work_time);
  uprintf("pomodoro_config.break_time: %d\n", temp_config.break_time);
  uprintf("pomodoro_config.long_break_time: %d\n", temp_config.long_break_time);
  uprintf("pomodoro_config.work_interval: %d\n", temp_config.work_interval);
  uprintf("pomodoro_config.work_hf_pattern: %d\n", temp_config.work_hf_pattern);
  uprintf("pomodoro_config.break_hf_pattern: %d\n", temp_config.break_hf_pattern);
  uprintf("pomodoro_config.notify_haptic_enable: %d\n", temp_config.notify_haptic_enable);
  uprintf("pomodoro_config.continuous_mode: %d\n", temp_config.continuous_mode);
  uprintf("pomodoro_config.pomodoro_cycle: %d\n", temp_config.pomodoro_cycle);
  uprintf("\n\n");

  update_pomodoro_config(temp_config);
}