typedef struct {
    bool init : 1;
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
} trackpad_config_t;
trackpad_config_t trackpad_config;

void init_trackpad_config(trackpad_config_t *trackpad_config);
void set_trackpad_config(trackpad_config_t trackpad_config);
void send_trackpad_config(const trackpad_config_t *config);
void receive_trackpad_config(uint8_t *data);