#pragma once
#include "gpk_rc.h"
#include "trackpad_config.h"
#include "pomodoro_config.h"

typedef struct {
    bool init : 1;
    trackpad_config_t trackpad_config;
    pomodoro_config_t pomodoro_config;
} device_config_t;

extern device_config_t device_config;

enum id_device_set_value_t {
    id_set_value_complete = 0x01,
    id_trackpad_set_value = 0x02,
    id_pomodoro_set_value = 0x03
};

enum id_device_get_value_t {
    id_device_get_value         = 0x01,
    id_trackpad_get_value       = 0x02,
    id_pomodoro_get_value       = 0x03,
    id_pomodoro_active_get_value = 0x04
};

enum id_device_operation_t {
    id_layer_move = 0x01,
    id_oled_write = 0x02
};

void init_device_config(device_config_t *device_config);
void save_device_config(device_config_t device_config);
