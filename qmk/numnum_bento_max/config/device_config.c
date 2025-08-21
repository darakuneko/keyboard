#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "drivers/haptic/drv2605l.h"
#include "device_config.h"
#include "trackpad_config.h"
#include "oled/oled_driver.h"

device_config_t device_config;

// Delayed save variables
static bool save_pending = false;
static uint32_t save_timer = 0;
static device_config_t pending_config;

void init_device_config(device_config_t *device_config) {
  eeconfig_init_user_datablock();
  init_trackpad_config(&device_config->trackpad_config);
  init_pomodoro_config(&device_config->pomodoro_config);
  init_led_config(&device_config->led_config);
  device_config->init = 1;
  schedule_device_config_save(*device_config);
}

void save_device_config(device_config_t device_config) {
  eeconfig_update_user_datablock(&device_config, 0, sizeof(device_config));
}

void schedule_device_config_save(device_config_t device_config) {
  pending_config = device_config;
  save_timer = timer_read32();
  save_pending = true;
}

void check_and_save_device_config(void) {
  if (save_pending && timer_elapsed32(save_timer) >= SAVE_DEVICE_CONFIG_TERM) {
    save_device_config(pending_config);
    save_pending = false;
  }
}

void send_set_value_complete(void) {
  uint8_t data[32] = {0};
  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_set_value;
  data[2] = id_set_value_complete;
  raw_hid_send(data, sizeof(data));
}

void send_device_config(void) {
  uint8_t data[32] = {0};
  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_get_value;
  data[2] = id_device_get_value;
  data[3] = 1;  // version
  data[4] = device_config.init;  // init flag
  const char device_name[] = "macropad_tp_btns";
  memcpy(&data[5], device_name, sizeof(device_name));
  raw_hid_send(data, sizeof(data));
}

void gpk_rc_handle_command_user(uint8_t id, uint8_t action, uint8_t *data, uint8_t length) {
  if(id == id_gpk_rc_set_value){
    if(action == id_trackpad_set_value) {
      receive_trackpad_config(data);
      send_set_value_complete();
    } else if(action == id_pomodoro_set_value) {
      receive_pomodoro_config(data);
      send_set_value_complete();
    } else if(action == id_led_set_value) {
      receive_led_config(data);
      send_set_value_complete();
    } else if(action == id_led_layer_set_value) {
      receive_led_layer_config(data);
      send_set_value_complete();
    }
  } else if(id == id_gpk_rc_get_value){
    if(action == id_device_get_value) {
      send_device_config();
    }
    if(action == id_trackpad_get_value) {
      send_trackpad_config(&device_config.trackpad_config);
    } else if(action == id_pomodoro_get_value) {
      send_pomodoro_config(&device_config.pomodoro_config);
    } else if(action == id_pomodoro_active_get_value) {
      send_pomodoro_active_status(&device_config.pomodoro_config);
    } else if(action == id_led_get_value) {
      send_led_config(&device_config.led_config);
    } else if(action == id_led_layer_get_value) {
      send_led_layer_config(&device_config.led_config);
    }
  } else if(id == id_gpk_rc_operation){
    if(action == id_layer_move) {
      layer_move(data[0]);
    } else if(action == id_oled_write) {
      #ifdef OLED_ENABLE
      if(is_oled_on()){
        oled_write((const char*) data, false);
      }
      #endif
    }
  }
}
