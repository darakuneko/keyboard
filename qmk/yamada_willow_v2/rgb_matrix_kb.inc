RGB_MATRIX_EFFECT(ikiri_breathing)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#include "action_layer.h"
uint16_t time = 0;
uint16_t breathing = 255;
bool ikiri_breathing(effect_params_t* params) {
  RGB_MATRIX_USE_LIMITS(led_min, led_max);
  
  HSV      hsv  = rgb_matrix_config.hsv;
  if(led_min == 0){
    time = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8);
  }
  hsv.v         = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
  RGB rgb       = rgb_matrix_hsv_to_rgb(hsv);
  uint8_t is_layer = get_highest_layer(layer_state);
  //int max_val = rgb_matrix_get_val();
  HSV hsv_b = {hsv.h, hsv.s, rgb_matrix_get_val()};
  if (is_layer == 1) {
    hsv_b.h = 191; //PURPLE
  } else if (is_layer == 2)  {
    hsv_b.h = 85; //GREEN
  } else if (is_layer == 3)  {
    hsv_b.h = 43; //YELLOW
  } else {
    hsv_b.h = 128; //CYAN
  }

  HSV hsv_r  = hsv;
  for (uint8_t i = led_min; i < led_max; i++) {
    if (HAS_FLAGS(g_led_config.flags[i], 0x04)) {
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    } else if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
      hsv_b.v         = scale8(abs8(sin8(time) - 96) * 2, hsv_b.v);
      RGB rgb_b       = rgb_matrix_hsv_to_rgb(hsv_b);
      rgb_matrix_set_color(i, rgb_b.r, rgb_b.g, rgb_b.b);
    } else if (HAS_FLAGS(g_led_config.flags[i], 0x01)) {
      if(led_min == 0) {
        breathing = hsv_r.v;
      }
      hsv_r.v         = scale8(abs8(sin8(time) - 128) * 2, breathing);
      breathing = hsv_r.v;
      RGB rgb_r       = rgb_matrix_hsv_to_rgb(hsv_r);
      rgb_matrix_set_color(i, rgb_r.r, rgb_r.g, rgb_r.b);
    }
  }
  return led_max < RGB_MATRIX_LED_COUNT;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS