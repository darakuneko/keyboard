#pragma once

typedef struct {
    // Layer color settings (max 9 layers including default)
    struct {
        uint8_t r, g, b;
    } layer_colors[9]; // RGB values (0-255)
    uint8_t layer_count; // Actual number of layers
    
    // Pomodoro color settings
    struct {
        uint8_t work_r, work_g, work_b;
        uint8_t break_r, break_g, break_b; 
        uint8_t long_break_r, long_break_g, long_break_b;
    } pomodoro_colors;
    
    // Speed/step indicator LED colors
    struct {
        uint8_t speed_r, speed_g, speed_b;
        uint8_t step_r, step_g, step_b;
    } indicator_colors;
} device_led_config_t;

extern device_led_config_t device_led_config;

void init_led_config(device_led_config_t *led_config);
void update_led_config(device_led_config_t led_config);
void set_led_config(device_led_config_t led_config);
void send_led_config(const device_led_config_t *config);
void send_led_layer_config(const device_led_config_t *config);
void receive_led_config(uint8_t *data);
void receive_led_layer_config(uint8_t *data);
uint8_t get_device_layer_count(void);

// Flash animation for pomodoro color changes
typedef enum {
    FLASH_NONE = 0,
    FLASH_WORK,
    FLASH_BREAK,
    FLASH_LONG_BREAK
} flash_type_t;

extern flash_type_t pomodoro_flash_type;
extern uint32_t pomodoro_flash_timer;
bool is_pomodoro_flashing(void);
RGB get_pomodoro_flash_color(void);