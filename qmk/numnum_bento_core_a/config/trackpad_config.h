#pragma once

typedef struct {
    unsigned int hf_waveform_number : 7;
    bool can_hf_for_layer : 1;
    bool can_drag : 1;
    unsigned int scroll_term : 10;
    unsigned int drag_term : 10;
    bool can_trackpad_layer : 1;
    bool can_reverse_scrolling_direction  : 1;
    bool drag_strength_mode : 1;
    unsigned int drag_strength : 5;
    unsigned int default_speed : 6;
    unsigned int scroll_step : 4;
    bool can_short_scroll: 1;
    unsigned int tap_term : 10;
    unsigned int swipe_term : 10;
    unsigned int pinch_term : 10;
    unsigned int gesture_term : 10;
    unsigned int short_scroll_term : 10;
    unsigned int zoom_distance : 16;
    bool can_reverse_h_scrolling_direction  : 1;
    bool can_high_res_scroll  : 1;
} trackpad_config_t;

// Configuration variables
extern trackpad_config_t trackpad_config;

// State variables - these should be accessed from iqs5xx.c
extern int trackpad_layer;
extern bool use_trackpad_layer;
extern bool use_drag;
extern float accel_speed;
extern int accel_step;
extern bool use_horizontal_scrolling;

void init_trackpad_config(trackpad_config_t *trackpad_config);
void update_trackpad_config(trackpad_config_t trackpad_config, bool should_save);
void set_trackpad_config(trackpad_config_t trackpad_config);
void send_trackpad_config(const trackpad_config_t *config);
void receive_trackpad_config(uint8_t *data);