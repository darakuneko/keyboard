/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "drivers/haptic/DRV2605L.h"
#include <stdio.h>
#include "iqs5xx.h"

typedef union {
  uint32_t raw;
  struct {
    bool  init_hf : 1;
    int  hf_mode : 10;
    bool layer_hf : 1;
    bool drag_mode : 1;
    uint32_t drag_time : 12;
    bool auto_trackpad_layer : 1;
  };
} user_config_t;
user_config_t user_config;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	
  [0] = LAYOUT(
    	MT(MOD_LALT,KC_ESC), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RALT,KC_BSLS), 
    	MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_LCTL,KC_QUOT),  
    	KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT, 
		                           KC_LGUI, MO(2),   LT(1, KC_SPC),       LT(1, KC_ENT), LT(2,KC_BSPC), KC_RGUI,
      LGUI(KC_TAB), LGUI(KC_TAB),
      LALT(KC_RGHT),     LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
      LCTL(KC_PPLS), LCTL(KC_PMNS),
      KC_ESC               
  ),

	[1] = LAYOUT(
		KC_GRV,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_LBRC, KC_RBRC,   KC_MINS, KC_EQL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
		KC_LSFT,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_NO,   KC_NO,   KC_NO,   KC_SCLN, KC_QUOT,  KC_LSFT,
		KC_NO,   MO(3),   KC_LSFT,  KC_LSFT,   MO(3),   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,  
    KC_TRNS
	),

	[2] = LAYOUT(
    KC_F1,   KC_F2,   KC_F3,    KC_F4,    KC_F5,   KC_F6,    KC_F7,   KC_F8,  KC_F9,  KC_F10,   KC_F11,  KC_F12,  
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS
	),

	[3] = LAYOUT(
		RGB_VAI,  RGB_SAI,    RGB_HUI,    RGB_SPI,   RGB_MOD,    RGB_TOG,  DT_PRNT,  DT_UP,   DT_DOWN,  KC_NO,   KC_NO,   KC_NO, 
		RGB_VAD,  RGB_SAD,    RGB_HUD,    RGB_SPD,   RGB_RMOD,   KC_NO,    KC_NO,    KC_NO,   KC_NO,    KC_NO,   KC_NO,   KC_NO,
		KC_F16,   KC_F17,     KC_F18,     KC_NO,     KC_NO,      KC_F19,   KC_F20,   KC_F21,  KC_F22,   KC_F23,  KC_NO,   KC_NO,
		KC_NO,   KC_NO,   EE_CLR,   QK_BOOT,   KC_NO,   KC_NO,
    LCTL(KC_DOWN),  LCTL(KC_UP), 
    LGUI(KC_RBRC),  LCTL(KC_RGHT),   LGUI(KC_LBRC), LCTL(KC_LEFT), 
    LGUI(KC_EQL),   LGUI(KC_MINS),
    KC_TRNS
	),

	[4] = LAYOUT(
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  
		KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO, 
		KC_LSFT,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_LSFT, 
		KC_NO,   KC_BTN2,   KC_BTN1,   KC_BTN1,   KC_BTN2,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS
	),
};

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  if(!user_config.init_hf) {
    user_config.init_hf = true;  
    user_config.layer_hf = true;
    user_config.hf_mode = 47;   
    user_config.drag_mode = true;  
    user_config.drag_time = 700;
    user_config.auto_trackpad_layer = false;
    eeconfig_update_user(user_config.raw); 
  }
  tap_mode = 1;
  hf_mode = user_config.hf_mode;
  is_layer_hf = user_config.layer_hf;
  is_drag_mode = user_config.drag_mode;  
  drag_time = user_config.drag_time;
  is_auto_trackpad_layer = user_config.auto_trackpad_layer;
}

void send_setting_string(int i){
  char buf[12]; 
  snprintf(buf, sizeof(buf), "%d", i);
  const char *s = buf;
  send_string(s);
}

void hf_DRV_pulse(bool ee2_up) {
  if (ee2_up) {
    user_config.hf_mode = hf_mode;  
    eeconfig_update_user(user_config.raw); 
    hf_mode = user_config.hf_mode;
  }
  DRV_pulse(hf_mode);
  send_setting_string(hf_mode);
}

void update_drag_time(uint32_t dt){
  user_config.drag_time = dt;
  eeconfig_update_user(user_config.raw); 
  drag_time = user_config.drag_time;
  send_setting_string(dt);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {

  switch (keycode) {  
      case KC_BTN1 ... KC_BTN5:
        if (record->event.pressed) {
          pointing_device_set_button(1 << (keycode - KC_BTN1));
        } else {
          pointing_device_clear_button(1 << (keycode - KC_BTN1));
        }
        return false;  
      case KC_F16: 
        if (record->event.pressed) {
          user_config.drag_mode = !is_drag_mode;  
          eeconfig_update_user(user_config.raw); 
          is_drag_mode = user_config.drag_mode;        
        }
        return false;             
      case KC_F17: 
        if (record->event.pressed) {
          drag_time = drag_time + 10;
          if(drag_time > 3000) {
              drag_time = 3000;
          }
          update_drag_time(drag_time);
        }
        return false;
      case KC_F18: 
        if (record->event.pressed) {
          if(drag_time != 0) {
            drag_time = drag_time - 10;
          }
          update_drag_time(drag_time);
        }
        return false;
     case KC_F19: 
        if (record->event.pressed) {
          user_config.layer_hf = !is_layer_hf;  
          eeconfig_update_user(user_config.raw); 
          is_layer_hf = user_config.layer_hf;        
        }
        return false;         
      case KC_F20: 
        if (record->event.pressed) {
            hf_DRV_pulse(false);
        }
        return false;
      case KC_F21: 
        if (record->event.pressed) {
            hf_mode++;
            if(hf_mode == 124) {
              hf_mode = 0;
            }
            hf_DRV_pulse(true);
        }
        return false;  
      case KC_F22: 
        if (record->event.pressed) {
            hf_mode--;
            if(hf_mode == -1) {
              hf_mode = 123;
            }
            hf_DRV_pulse(true);
        }
        return false;
     case KC_F23: 
        if (record->event.pressed) {
          user_config.auto_trackpad_layer = !is_auto_trackpad_layer;  
          eeconfig_update_user(user_config.raw); 
          is_auto_trackpad_layer = user_config.auto_trackpad_layer;        
        }
        return false;    
    default:
      return true;
  }
};

int layer = 0;

void matrix_scan_user(void) {
  int current_layer = get_highest_layer(layer_state|default_layer_state); 
  if(!is_auto_trackpad_layer && is_layer_hf && layer != current_layer){
    DRV_pulse(hf_mode);
    layer = current_layer;
  }
}

bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    return false;
}

led_config_t g_led_config = { {
  // Key Matrix to LED Index
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, }, 
    { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, }, 
    { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, }, 
    { NO_LED, NO_LED, NO_LED, 36, 37, 38, 39, 40, 41 }
}, {
  // LED Index to Physical Position 
  {0, 0},     {20, 0},     {40, 0},     {60, 0},     {80, 0},     {100, 0},    {120, 0},     {140, 0},     {160, 0},     {180, 0},     {200, 0},     {224, 0},  
  {0, 21},    {20, 21},    {40, 21},    {60, 21},    {80, 21},    {100, 21},   {120, 21},    {140, 21},    {160, 21},    {180, 21},    {200, 21},    {224, 21},  
  {0, 42},    {20, 42},    {40, 42},    {60, 42},    {80, 42},    {100, 42},   {120, 42},    {140, 42},    {160, 42},    {180, 42},    {200, 42},    {224, 42},  
                                        {60, 64},    {80, 64},    {100, 64},   {120, 64},    {140, 64},    {160, 64}
  }, {
  // LED Index to Flag
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
  4, 4, 4, 4, 4, 4, 
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2
  } 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
  int current_layer = get_highest_layer(layer_state|default_layer_state);  
  HSV hsv = {0, 255, rgblight_get_val()};
  if (current_layer == 1) {
    hsv.h = 191; //PURPLE
  } else if (current_layer == 2)  {
    hsv.h = 85; //GREEN
  } else if (current_layer == 3)  {
    hsv.h = 43; //YELLOW
  } else if (current_layer == 4)  {
    hsv.h = 255; //RED
  } else {
    hsv.h = 128; //CYAN
  }
  RGB rgb = hsv_to_rgb(hsv);

  
  for (uint8_t i = led_min; i <= led_max -1; i++) {
      if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
      }
  }
  //return false;
}  