#include QMK_KEYBOARD_H
#include "raw_hid.h"
#include "led_config.h"
#include "device_config.h"

device_led_config_t device_led_config;

// Flash animation variables
flash_type_t pomodoro_flash_type = FLASH_NONE;
uint32_t pomodoro_flash_timer = 0;

void init_led_config(device_led_config_t *led_config) {
    // Initialize layer colors with HSV to RGB conversion
    HSV hsv;
    RGB rgb;
    
    // Layer 0: CYAN (default)
    hsv = (HSV){128, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[0].r = rgb.r;
    led_config->layer_colors[0].g = rgb.g;
    led_config->layer_colors[0].b = rgb.b;
    
    // Layer 1: MAGENTA
    hsv = (HSV){213, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[1].r = rgb.r;
    led_config->layer_colors[1].g = rgb.g;
    led_config->layer_colors[1].b = rgb.b;
    
    // Layer 2: GREEN
    hsv = (HSV){85, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[2].r = rgb.r;
    led_config->layer_colors[2].g = rgb.g;
    led_config->layer_colors[2].b = rgb.b;
    
    // Layer 3: BLUE
    hsv = (HSV){170, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[3].r = rgb.r;
    led_config->layer_colors[3].g = rgb.g;
    led_config->layer_colors[3].b = rgb.b;
    
    // Layer 4: CORAL
    hsv = (HSV){11, 176, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[4].r = rgb.r;
    led_config->layer_colors[4].g = rgb.g;
    led_config->layer_colors[4].b = rgb.b;
    
    // Layer 5: YELLOW
    hsv = (HSV){43, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[5].r = rgb.r;
    led_config->layer_colors[5].g = rgb.g;
    led_config->layer_colors[5].b = rgb.b;
    
    // Layer 6: ORANGE
    hsv = (HSV){10, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[6].r = rgb.r;
    led_config->layer_colors[6].g = rgb.g;
    led_config->layer_colors[6].b = rgb.b;
    
    // Layer 7: RED
    hsv = (HSV){0, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[7].r = rgb.r;
    led_config->layer_colors[7].g = rgb.g;
    led_config->layer_colors[7].b = rgb.b;
    
    // Layer 8: PURPLE
    hsv = (HSV){191, 255, 255};
    rgb = hsv_to_rgb(hsv);
    led_config->layer_colors[8].r = rgb.r;
    led_config->layer_colors[8].g = rgb.g;
    led_config->layer_colors[8].b = rgb.b;
    
    extern int end_layer;
    led_config->layer_count = end_layer + 1;  // Dynamic layer count (0 to end_layer)
    
    // Initialize pomodoro colors
    // Work: Red
    led_config->pomodoro_colors.work_r = 255;
    led_config->pomodoro_colors.work_g = 0;
    led_config->pomodoro_colors.work_b = 0;
    
    // Break: Green
    led_config->pomodoro_colors.break_r = 0;
    led_config->pomodoro_colors.break_g = 255;
    led_config->pomodoro_colors.break_b = 0;
    
    // Long Break: Blue
    led_config->pomodoro_colors.long_break_r = 0;
    led_config->pomodoro_colors.long_break_g = 0;
    led_config->pomodoro_colors.long_break_b = 255;
    
    // Initialize indicator colors
    // Speed indicator: Red
    led_config->indicator_colors.speed_r = 255;
    led_config->indicator_colors.speed_g = 0;
    led_config->indicator_colors.speed_b = 0;
    
    // Step indicator: Red
    led_config->indicator_colors.step_r = 255;
    led_config->indicator_colors.step_g = 0;
    led_config->indicator_colors.step_b = 0;

    // horizontal Scroll indicator: Red
    led_config->indicator_colors.h_scroll_r = 255;
    led_config->indicator_colors.h_scroll_g = 0;
    led_config->indicator_colors.h_scroll_b = 0;
}

void set_led_config(device_led_config_t config) {
    device_led_config = config;
}

void update_led_config(device_led_config_t config) {    
    //internal
    set_led_config(config);
    
    // Update global device_led_config
    device_config.led_config = config;
    // Schedule delayed save to EEPROM via device_config
    schedule_device_config_save(device_config);
}

void send_led_config(const device_led_config_t *config) {
    uint8_t data[32] = {0};
    data[0] = id_gpk_rc_prefix;
    data[1] = id_gpk_rc_get_value;
    data[2] = id_led_get_value;
    
    // Pack pomodoro colors
    data[3] = config->pomodoro_colors.work_r;
    data[4] = config->pomodoro_colors.work_g;
    data[5] = config->pomodoro_colors.work_b;
    data[6] = config->pomodoro_colors.break_r;
    data[7] = config->pomodoro_colors.break_g;
    data[8] = config->pomodoro_colors.break_b;
    data[9] = config->pomodoro_colors.long_break_r;
    data[10] = config->pomodoro_colors.long_break_g;
    data[11] = config->pomodoro_colors.long_break_b;
    
    // Pack indicator colors
    data[12] = config->indicator_colors.speed_r;
    data[13] = config->indicator_colors.speed_g;
    data[14] = config->indicator_colors.speed_b;
    data[15] = config->indicator_colors.step_r;
    data[16] = config->indicator_colors.step_g;
    data[17] = config->indicator_colors.step_b;
    data[18] = config->indicator_colors.h_scroll_r;
    data[19] = config->indicator_colors.h_scroll_g;
    data[20] = config->indicator_colors.h_scroll_b;
    
    uprintf("pomodoro_colors.work_r: %d\n", config->pomodoro_colors.work_r);
    uprintf("pomodoro_colors.work_g: %d\n", config->pomodoro_colors.work_g);
    uprintf("pomodoro_colors.work_b: %d\n", config->pomodoro_colors.work_b);
    uprintf("pomodoro_colors.break_r: %d\n", config->pomodoro_colors.break_r);
    uprintf("pomodoro_colors.break_g: %d\n", config->pomodoro_colors.break_g);
    uprintf("pomodoro_colors.break_b: %d\n", config->pomodoro_colors.break_b);
    uprintf("pomodoro_colors.long_break_r: %d\n", config->pomodoro_colors.long_break_r);
    uprintf("pomodoro_colors.long_break_g: %d\n", config->pomodoro_colors.long_break_g);
    uprintf("pomodoro_colors.long_break_b: %d\n", config->pomodoro_colors.long_break_b);
    uprintf("indicator_colors.speed_r: %d\n", config->indicator_colors.speed_r);
    uprintf("indicator_colors.speed_g: %d\n", config->indicator_colors.speed_g);
    uprintf("indicator_colors.speed_b: %d\n", config->indicator_colors.speed_b);
    uprintf("indicator_colors.step_r: %d\n", config->indicator_colors.step_r);
    uprintf("indicator_colors.step_g: %d\n", config->indicator_colors.step_g);
    uprintf("indicator_colors.step_b: %d\n", config->indicator_colors.step_b);
    uprintf("indicator_colors.h_scroll_r: %d\n", config->indicator_colors.h_scroll_r);
    uprintf("indicator_colors.h_scroll_b: %d\n", config->indicator_colors.h_scroll_b);
    uprintf("indicator_colors.h_scroll_g: %d\n", config->indicator_colors.h_scroll_g);
    uprintf("\n\n");
    
    raw_hid_send(data, sizeof(data));
}

void send_led_layer_config(const device_led_config_t *config) {
    uint8_t data[32] = {0};
    data[0] = id_gpk_rc_prefix;
    data[1] = id_gpk_rc_get_value;
    data[2] = id_led_layer_get_value;
    
    // Pack layer count
    data[3] = get_device_layer_count();
    
    // Pack layer colors
    int max_layers = sizeof(config->layer_colors) / sizeof(config->layer_colors[0]);
    for (int i = 0; i < config->layer_count && i < max_layers; i++) {
        data[4 + i * 3] = config->layer_colors[i].r;
        data[5 + i * 3] = config->layer_colors[i].g;
        data[6 + i * 3] = config->layer_colors[i].b;
    }
    
    uprintf("layer_count: %d\n", get_device_layer_count());
    for (int i = 0; i < config->layer_count && i < max_layers; i++) {
        uprintf("layer_colors[%d]: r=%d g=%d b=%d\n", i, config->layer_colors[i].r, config->layer_colors[i].g, config->layer_colors[i].b);
    }
    uprintf("\n\n");
    
    raw_hid_send(data, sizeof(data));
}

void receive_led_config(uint8_t *data) {
    device_led_config_t received_config = device_led_config;
    
    // Store old colors for comparison
    uint8_t old_work_r = device_led_config.pomodoro_colors.work_r;
    uint8_t old_work_g = device_led_config.pomodoro_colors.work_g;
    uint8_t old_work_b = device_led_config.pomodoro_colors.work_b;
    uint8_t old_break_r = device_led_config.pomodoro_colors.break_r;
    uint8_t old_break_g = device_led_config.pomodoro_colors.break_g;
    uint8_t old_break_b = device_led_config.pomodoro_colors.break_b;
    uint8_t old_long_break_r = device_led_config.pomodoro_colors.long_break_r;
    uint8_t old_long_break_g = device_led_config.pomodoro_colors.long_break_g;
    uint8_t old_long_break_b = device_led_config.pomodoro_colors.long_break_b;
    
    // Unpack pomodoro colors
    received_config.pomodoro_colors.work_r = data[0];
    received_config.pomodoro_colors.work_g = data[1];
    received_config.pomodoro_colors.work_b = data[2];
    received_config.pomodoro_colors.break_r = data[3];
    received_config.pomodoro_colors.break_g = data[4];
    received_config.pomodoro_colors.break_b = data[5];
    received_config.pomodoro_colors.long_break_r = data[6];
    received_config.pomodoro_colors.long_break_g = data[7];
    received_config.pomodoro_colors.long_break_b = data[8];
    
    // Unpack indicator colors
    received_config.indicator_colors.speed_r = data[9];
    received_config.indicator_colors.speed_g = data[10];
    received_config.indicator_colors.speed_b = data[11];
    received_config.indicator_colors.step_r = data[12];
    received_config.indicator_colors.step_g = data[13];
    received_config.indicator_colors.step_b = data[14];
    received_config.indicator_colors.h_scroll_r = data[15];
    received_config.indicator_colors.h_scroll_g = data[16];
    received_config.indicator_colors.h_scroll_b = data[17];  
    uprintf("led_config.pomodoro_colors.work_r: %d\n", received_config.pomodoro_colors.work_r);
    uprintf("led_config.pomodoro_colors.work_g: %d\n", received_config.pomodoro_colors.work_g);
    uprintf("led_config.pomodoro_colors.work_b: %d\n", received_config.pomodoro_colors.work_b);
    uprintf("led_config.pomodoro_colors.break_r: %d\n", received_config.pomodoro_colors.break_r);
    uprintf("led_config.pomodoro_colors.break_g: %d\n", received_config.pomodoro_colors.break_g);
    uprintf("led_config.pomodoro_colors.break_b: %d\n", received_config.pomodoro_colors.break_b);
    uprintf("led_config.pomodoro_colors.long_break_r: %d\n", received_config.pomodoro_colors.long_break_r);
    uprintf("led_config.pomodoro_colors.long_break_g: %d\n", received_config.pomodoro_colors.long_break_g);
    uprintf("led_config.pomodoro_colors.long_break_b: %d\n", received_config.pomodoro_colors.long_break_b);
    uprintf("led_config.indicator_colors.speed_r: %d\n", received_config.indicator_colors.speed_r);
    uprintf("led_config.indicator_colors.speed_g: %d\n", received_config.indicator_colors.speed_g);
    uprintf("led_config.indicator_colors.speed_b: %d\n", received_config.indicator_colors.speed_b);
    uprintf("led_config.indicator_colors.step_r: %d\n", received_config.indicator_colors.step_r);
    uprintf("led_config.indicator_colors.step_g: %d\n", received_config.indicator_colors.step_g);
    uprintf("led_config.indicator_colors.step_b: %d\n", received_config.indicator_colors.step_b);
    uprintf("led_config.indicator_colors.h_scroll_r: %d\n", received_config.indicator_colors.h_scroll_r);
    uprintf("led_config.indicator_colors.h_scroll_g: %d\n", received_config.indicator_colors.h_scroll_g);
    uprintf("led_config.indicator_colors.h_scroll_b: %d\n", received_config.indicator_colors.h_scroll_b);
    uprintf("\n\n");
    
    // Check for color changes and trigger flash
    if (old_work_r != received_config.pomodoro_colors.work_r ||
        old_work_g != received_config.pomodoro_colors.work_g ||
        old_work_b != received_config.pomodoro_colors.work_b) {
        pomodoro_flash_type = FLASH_WORK;
        pomodoro_flash_timer = timer_read32();
        uprintf("Work color changed - flashing!\n");
    } else if (old_break_r != received_config.pomodoro_colors.break_r ||
               old_break_g != received_config.pomodoro_colors.break_g ||
               old_break_b != received_config.pomodoro_colors.break_b) {
        pomodoro_flash_type = FLASH_BREAK;
        pomodoro_flash_timer = timer_read32();
        uprintf("Break color changed - flashing!\n");
    } else if (old_long_break_r != received_config.pomodoro_colors.long_break_r ||
               old_long_break_g != received_config.pomodoro_colors.long_break_g ||
               old_long_break_b != received_config.pomodoro_colors.long_break_b) {
        pomodoro_flash_type = FLASH_LONG_BREAK;
        pomodoro_flash_timer = timer_read32();
        uprintf("Long break color changed - flashing!\n");
    }
    
    update_led_config(received_config);
}

void receive_led_layer_config(uint8_t *data) {
    device_led_config_t received_config = device_led_config;
    
    // Unpack layer configuration
    received_config.layer_count = data[0];
    for (int i = 0; i < received_config.layer_count; i++) {
        received_config.layer_colors[i].r = data[1 + i * 3];
        received_config.layer_colors[i].g = data[2 + i * 3];
        received_config.layer_colors[i].b = data[3 + i * 3];
    }
    
    uprintf("led_config.layer_count: %d\n", received_config.layer_count);
    for (int i = 0; i < received_config.layer_count; i++) {
        uprintf("led_config.layer_colors[%d]: r=%d g=%d b=%d\n", i, received_config.layer_colors[i].r, received_config.layer_colors[i].g, received_config.layer_colors[i].b);
    }
    uprintf("\n\n");
    
    update_led_config(received_config);
}

uint8_t get_device_layer_count(void) {
    extern int end_layer;
    return end_layer + 1;
}

bool is_pomodoro_flashing(void) {
    if (pomodoro_flash_type == FLASH_NONE) {
        return false;
    }
    
    // Flash for 3 seconds
    if (timer_elapsed32(pomodoro_flash_timer) >= 3000) {
        pomodoro_flash_type = FLASH_NONE;
        return false;
    }
    
    return true;
}

RGB get_pomodoro_flash_color(void) {
    RGB color = {0, 0, 0};
    
    switch (pomodoro_flash_type) {
        case FLASH_WORK:
            color.r = device_led_config.pomodoro_colors.work_r;
            color.g = device_led_config.pomodoro_colors.work_g;
            color.b = device_led_config.pomodoro_colors.work_b;
            break;
        case FLASH_BREAK:
            color.r = device_led_config.pomodoro_colors.break_r;
            color.g = device_led_config.pomodoro_colors.break_g;
            color.b = device_led_config.pomodoro_colors.break_b;
            break;
        case FLASH_LONG_BREAK:
            color.r = device_led_config.pomodoro_colors.long_break_r;
            color.g = device_led_config.pomodoro_colors.long_break_g;
            color.b = device_led_config.pomodoro_colors.long_break_b;
            break;
        default:
            break;
    }
    
    return color;
}