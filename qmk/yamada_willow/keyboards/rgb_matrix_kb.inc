RGB_MATRIX_EFFECT(IKIRIBREATHING)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool IKIRIBREATHING(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    HSV      hsv  = rgb_matrix_config.hsv;
    uint16_t time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
    hsv.v         = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
    RGB rgb       = rgb_matrix_hsv_to_rgb(hsv);

    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv_b = {hsv.h, hsv.s, rgblight_get_val()};
    if (is_layer == 1) {
      hsv_b.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv_b.h = 85; //GREEN
    } else if (is_layer == 3)  {
      hsv_b.h = 43; //YELLOW
    } else {
      hsv_b.h = 191; //PURPLE
    }

    HSV      hsv_r  = hsv;
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x04)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          hsv_b.v         = scale8(abs8(sin8(time) - 96) * 2, hsv_b.v);
          RGB rgb       = rgb_matrix_hsv_to_rgb(hsv_b);
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else if (HAS_FLAGS(g_led_config.flags[i], 0x01)) {
          hsv_r.v         = scale8(abs8(sin8(time) - 128) * 2, hsv_r.v);
          RGB rgb       = rgb_matrix_hsv_to_rgb(hsv_r);
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return led_max < DRIVER_LED_TOTAL;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS