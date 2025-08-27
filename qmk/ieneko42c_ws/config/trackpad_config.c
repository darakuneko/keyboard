#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "drivers/haptic/drv2605l.h"
#include "../device/iqs5xx.h"
#include "device_config.h"
#include "trackpad_config.h"

// Configuration structure
trackpad_config_t trackpad_config;

// State variables
int trackpad_layer;
bool use_trackpad_layer;
bool use_drag;
float accel_speed;
int accel_step;
bool use_horizontal_scrolling;

void init_trackpad_config(trackpad_config_t *trackpad_config) {
  trackpad_config->hf_waveform_number = 48;   
  trackpad_config->can_hf_for_layer = true;
  trackpad_config->can_drag = true;  
  trackpad_config->can_trackpad_layer = true;
  trackpad_config->can_reverse_scrolling_direction = false;
  trackpad_config->can_reverse_h_scrolling_direction = false;
  trackpad_config->scroll_term = 100;
  trackpad_config->drag_term = 500;
  trackpad_config->drag_strength_mode = false;
  trackpad_config->drag_strength = 6;
  trackpad_config->default_speed = 10;
#ifdef  POINTING_DEVICE_HIRES_SCROLL_ENABLE
  trackpad_config->scroll_step = 7;
#else
  trackpad_config->scroll_step = 0;
#endif  
  trackpad_config->can_short_scroll = true;
  trackpad_config->tap_term = 200;
  trackpad_config->swipe_term = 150;
  trackpad_config->pinch_term = 300;
  trackpad_config->gesture_term = 300;
  trackpad_config->short_scroll_term = 70;
  trackpad_config->zoom_distance = 400;
}

void update_trackpad_config(trackpad_config_t temp_confing, bool should_save) {
  if(should_save) {
    if(trackpad_config.hf_waveform_number != temp_confing.hf_waveform_number){
      drv2605l_pulse(temp_confing.hf_waveform_number);
    }
    schedule_device_config_save(device_config);
  }
  set_iqs5xx_zoom_distance(temp_confing.zoom_distance);
  //global
  device_config.trackpad_config = temp_confing;
  //internal iqs5xx
  trackpad_config = temp_confing;
}

void set_trackpad_config(trackpad_config_t temp_config) {
  update_trackpad_config(temp_config, false);
  set_iqs5xx_zoom_distance(temp_config.zoom_distance);
  accel_speed = 1;
  accel_step = 1;
  trackpad_layer = 4;
  use_trackpad_layer = false;
  use_drag = false;
  use_horizontal_scrolling = false;
}

void send_trackpad_config(const trackpad_config_t *config) {
  uint8_t data[32] = {0};

  data[0] = id_gpk_rc_prefix;
  data[1] = id_gpk_rc_get_value;
  data[2] = id_trackpad_get_value;
  
  int upper_scroll_term = (config->scroll_term & 0b1111110000) >> 4;
  int lower_drag_term = (config->drag_term & 0b1111000000) >> 6;
  int lower_default_speed = (config->default_speed & 0b110000) >> 4;

  data[3] = config->hf_waveform_number;

  data[4] = config->can_hf_for_layer << 7 |
            config->can_drag << 6 |
            upper_scroll_term;

  data[5] = (config->scroll_term & 0b0000001111) << 4 |
            lower_drag_term;

  data[6] = (config->drag_term & 0b0000111111) << 2 |
            config->can_trackpad_layer << 1 |
            config->can_reverse_scrolling_direction;

  data[7] = config->drag_strength_mode << 7 |
            config->drag_strength << 2 |
            lower_default_speed;

  data[8] = (config->default_speed & 0b001111) << 4 |
            config->scroll_step; 

  data[9] = config->can_short_scroll << 7 |
            config->can_reverse_h_scrolling_direction << 6;
  
  data[10] = config->tap_term >> 8;      
  data[11] = config->tap_term & 0xFF;   
  data[12] = config->swipe_term >> 8;    
  data[13] = config->swipe_term & 0xFF; 
  data[14] = config->pinch_term >> 8;    
  data[15] = config->pinch_term & 0xFF; 
  data[16] = config->gesture_term >> 8;  
  data[17] = config->gesture_term & 0xFF;
  data[18] = config->short_scroll_term >> 8;    
  data[19] = config->short_scroll_term & 0xFF; 
  data[20] = config->zoom_distance >> 8;
  data[21] = config->zoom_distance & 0xFF;
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
  const uint8_t upper2Bits = a & 0b00000011;
  const uint8_t lower4Bits = (b & 0b11110000) >> 4;
  return (upper2Bits << 4) | lower4Bits;
}

void receive_trackpad_config(uint8_t *data) {
  trackpad_config_t temp_config;

  temp_config.hf_waveform_number = data[0];
  temp_config.can_hf_for_layer = (data[1] & 0b10000000) >> 7;
  temp_config.can_drag = (data[1] & 0b01000000) >> 6;
  temp_config.scroll_term = joinScrollTerm(data[1], data[2]);
  temp_config.drag_term = joinDragTerm(data[2], data[3]);
  temp_config.can_trackpad_layer = (data[3] & 0b00000010) >> 1;
  temp_config.can_reverse_scrolling_direction = data[3] & 0b00000001;
  temp_config.drag_strength_mode = (data[4] & 0b10000000) >> 7;
  temp_config.drag_strength = (data[4] & 0b01111100) >> 2;
  temp_config.default_speed = joinDefaultSpeed(data[4], data[5]);
  temp_config.scroll_step = data[5] & 0b00001111;
  temp_config.can_short_scroll = (data[6] & 0b10000000) >> 7;
  temp_config.can_reverse_h_scrolling_direction = (data[6] & 0b01000000) >> 6;
  temp_config.tap_term = (data[7] << 8) | data[8];           
  temp_config.swipe_term = (data[9] << 8) | data[10];        
  temp_config.pinch_term = (data[11] << 8) | data[12];       
  temp_config.gesture_term = (data[13] << 8) | data[14];     
  temp_config.short_scroll_term = (data[15] << 8) | data[16];
  temp_config.zoom_distance = (data[17] << 8) | data[18];

  uprintf("trackpad_config.hf_waveform_number: %d\n", temp_config.hf_waveform_number);
  uprintf("trackpad_config.can_hf_for_layer: %d\n", temp_config.can_hf_for_layer);
  uprintf("trackpad_config.can_drag: %d\n", temp_config.can_drag); 
  uprintf("trackpad_config.scroll_term: %d\n", temp_config.scroll_term);
  uprintf("trackpad_config.drag_term: %d\n", temp_config.drag_term);
  uprintf("trackpad_config.can_trackpad_layer: %d\n", temp_config.can_trackpad_layer);
  uprintf("trackpad_config.can_reverse_scrolling_direction: %d\n", temp_config.can_reverse_scrolling_direction);
  uprintf("trackpad_config.drag_strength_mode: %d\n", temp_config.drag_strength_mode);
  uprintf("trackpad_config.drag_strength: %d\n", temp_config.drag_strength);
  uprintf("trackpad_config.default_speed: %d\n", temp_config.default_speed);
  uprintf("trackpad_config.scroll_step: %d\n", temp_config.scroll_step);
  uprintf("trackpad_config.can_short_scroll: %d\n", temp_config.can_short_scroll);
  uprintf("trackpad_config.tap_term: %d\n", temp_config.tap_term);
  uprintf("trackpad_config.swipe_term: %d\n", temp_config.swipe_term);
  uprintf("trackpad_config.pinch_term: %d\n", temp_config.pinch_term);
  uprintf("trackpad_config.gesture_term: %d\n", temp_config.gesture_term);
  uprintf("trackpad_config.short_scroll_term: %d\n", temp_config.short_scroll_term);
  uprintf("trackpad_config.zoom_distance: %d\n", temp_config.zoom_distance);
  uprintf("trackpad_config.can_reverse_h_scrolling_direction: %d\n", temp_config.can_reverse_h_scrolling_direction);
  uprintf("\n\n");          
  
  update_trackpad_config(temp_config, true);
}
