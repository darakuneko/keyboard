#include "device_config.h"

void send_device_config(void) {
  uint8_t data[32] = {0};
  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_get_value;
  data[2] = id_device_get_value; // action
  data[3] = 1; // version
  data[4] = 1;  // init flag
  const char device_name[] = "keyboard_oled";
  memcpy(&data[5], device_name, sizeof(device_name));
  raw_hid_send(data, sizeof(data));
}

void gpk_rc_handle_command_user(uint8_t id, uint8_t action, uint8_t *data, uint8_t length) {
  if(id == id_gpk_rc_get_value){
      send_device_config();
  } else if(id == id_gpk_rc_operation){
    if(action == 0x01) {
      layer_move(data[0]);
    } else if(action == 0x02) {
      #ifdef OLED_ENABLE
      if(is_oled_on()){
        oled_write((const char*) data, false);
      }
      #endif
    }
  }
}
