#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "drivers/haptic/drv2605l.h"
#include "iqs5xx.h"
#include "trackpad_config.h"

void init_trackpad_config(trackpad_config_t *trackpad_config) {
  trackpad_config->init = true;  
  trackpad_config->hf_waveform_number = 48;   
  trackpad_config->can_hf_for_layer = true;
  trackpad_config->can_drag = true;  
  trackpad_config->can_trackpad_layer = false;
  trackpad_config->can_reverse_scrolling_direction = false;
  trackpad_config->scroll_term = 100;
  trackpad_config->drag_term = 500;
  trackpad_config->drag_strength_mode = false;
  trackpad_config->drag_strength = 6;
  trackpad_config->default_speed = 10;
  trackpad_config->scroll_step = 0;
  eeconfig_update_user_datablock(&trackpad_config); 
}

void update_trackpad_config(trackpad_config_t trackpad_config) {
  if(hf_waveform_number != trackpad_config.hf_waveform_number){
     drv2605l_pulse(trackpad_config.hf_waveform_number);
  }
  hf_waveform_number = trackpad_config.hf_waveform_number;
  can_hf_for_layer = trackpad_config.can_hf_for_layer;
  can_drag = trackpad_config.can_drag;  
  can_reverse_scrolling_direction = trackpad_config.can_reverse_scrolling_direction;
  drag_term = trackpad_config.drag_term;
  scroll_term = trackpad_config.scroll_term;
  drag_strength_mode = trackpad_config.drag_strength_mode;
  drag_strength = trackpad_config.drag_strength;
  default_speed = (double)trackpad_config.default_speed / 10.0;
  scroll_step = trackpad_config.scroll_step + 1;
}

void set_trackpad_config(trackpad_config_t trackpad_config) {
  update_trackpad_config(trackpad_config);
  accel_speed = 1;
  accel_step = 1;
  trackpad_layer = 5;
  use_trackpad_layer = false;
  use_drag = false;
}

void send_trackpad_config(const trackpad_config_t *config) {
  uint8_t data[32]; 
  memset(data, 0, sizeof(data));
    
  const char* identifier = "gpktps";
  for (int i = 0; i < strlen(identifier); i++) {
      data[i] = (uint8_t)identifier[i];
  }

  int upper_scroll_term = (config->scroll_term & 0b1111110000) >> 4;
  int lower_drag_term = (config->drag_term & 0b1111000000) >> 6;
  int lower_default_speed = (config->default_speed & 0b110000) >> 4;

  data[6] = config->init | config->hf_waveform_number << 1;

  data[7] = config->can_hf_for_layer << 7 |
            config->can_drag << 6 |
            upper_scroll_term;

  data[8] = (config->scroll_term & 0b0000001111) << 4 |
            lower_drag_term;

  data[9] = (config->drag_term & 0b0000111111) << 2 |
            config->can_trackpad_layer << 1 |
            config->can_reverse_scrolling_direction;

  data[10] = config->drag_strength_mode << 7 |
             config->drag_strength << 2 |
             lower_default_speed;

  data[11] = (config->default_speed & 0b001111) << 4 |
             config->scroll_step; 

  raw_hid_send(data, sizeof(data));
}

unsigned int joinScrollTerm(uint8_t a, uint8_t b) {
  const uint8_t lower6Bits = a & 0b00111111;
  const uint8_t upper4Bits = (b & 0b11110000) >> 4;
  return (lower6Bits << 4) | upper4Bits;
}

unsigned int joinDragTerm(uint8_t a, uint8_t b) {
  const uint8_t lower4Bits = a & 0b00001111;
  const uint8_t upper6Bits = (b & 0b11111100) >> 2;
  return (lower4Bits << 6) | upper6Bits;
}

unsigned int joinDefaultSpeed(uint8_t a, uint8_t b) {
  const uint8_t lower2Bits = a & 0b00000011;
  const uint8_t upper4Bits = (b & 0b11110000) >> 4;
  return (lower2Bits << 4) | upper4Bits;
}

void receive_trackpad_config(uint8_t *data) {
  trackpad_config.init = data[1] & 0b0000001;
  trackpad_config.hf_waveform_number = (data[1] & 0b11111110) >> 1;
  trackpad_config.can_hf_for_layer = (data[2] & 0b10000000) >> 7;
  trackpad_config.can_drag = (data[2] & 0b01000000) >> 6;
  trackpad_config.scroll_term = joinScrollTerm(data[2], data[3]);
  trackpad_config.drag_term = joinDragTerm(data[3], data[4]);
  trackpad_config.can_trackpad_layer = (data[4] & 0b00000010) >> 1;
  trackpad_config.can_reverse_scrolling_direction = data[4] & 0b00000001;
  trackpad_config.drag_strength_mode = (data[5] & 0b10000000) >> 7;
  trackpad_config.drag_strength = (data[5] & 0b01111100) >> 2;
  trackpad_config.default_speed = joinDefaultSpeed(data[5], data[6]);
  trackpad_config.scroll_step = data[6] & 0b00001111;
  eeconfig_update_user_datablock(&trackpad_config); 
  update_trackpad_config(trackpad_config);
}
