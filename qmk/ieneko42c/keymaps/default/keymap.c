/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include "drivers/haptic/DRV2605L.h"
#include <stdio.h>
#include "iqs5xx.h"

typedef union {
  uint32_t raw;
  struct {
    bool init : 1;
    int  tap_mode : 1;
    int  hf_mode : 8;
    bool layer_hf : 1;
    bool drag_mode : 1;
    int  scroll_threshold_time : 8;
    uint32_t drag_time : 12;
    bool auto_trackpad_layer : 1;
  };
} user_config_t;
user_config_t user_config;

enum {
  SCLL_UP = SAFE_RANGE,
  SCLL_DOWN,
  HF_UP,
  HF_DOWN,
  DRG_UP,
  DRG_DOWN
};

#define DOUBLE_TAP_TIME_MS 200

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	
  [0] = LAYOUT(
    	MT(MOD_LALT,KC_ESC), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RALT,KC_BSLS), 
    	MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_LCTL,KC_QUOT),  
    	KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT, 
		                           KC_LGUI, LT(2, KC_DEL),   LT(1, KC_SPC),       LT(1, KC_ENT), LT(2,KC_BSPC), KC_RGUI,
      LGUI(KC_TAB), LGUI(KC_TAB),
      LALT(KC_RGHT),     LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
      LCTL(KC_PPLS), LCTL(KC_PMNS),
      KC_ESC               
  ),

	[1] = LAYOUT(
		KC_GRV,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,  KC_NO,   KC_NO,   KC_LBRC, KC_RBRC,   KC_MINS, KC_EQL,
		KC_NO,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_NO,   KC_NO,
		KC_LSFT,   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,   KC_NO,   KC_NO,   KC_NO,   KC_SCLN, KC_QUOT,  KC_LSFT,
		KC_ACL0,   MO(3),   KC_LSFT,  KC_LSFT,   MO(3),   KC_ACL0,
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
		TD(0),    KC_F19,     KC_F20,     TD(1),     KC_F21,     TD(2),    KC_F22,   KC_F23,  KC_NO,    KC_NO,   KC_NO,   KC_NO,
		KC_NO,   KC_NO,   EE_CLR,   QK_BOOT,   KC_NO,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS
	),

	[4] = LAYOUT(
    MT(MOD_LALT,KC_ESC), KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    MT(MOD_RALT,KC_BSLS), 
    MT(MOD_LCTL,KC_TAB), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, MT(MOD_LCTL,KC_QUOT),  
    KC_LSFT,             KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_LSFT, 
		                        KC_NO,   KC_ACL0,   KC_BTN1,   KC_BTN1,   KC_ACL0,   KC_NO,
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS
	),
};

uint32_t init_opts(user_config_t* user_config) {
  user_config->init = true;  
  user_config->layer_hf = true;
  user_config->tap_mode = true;   
  user_config->hf_mode = 47;   
  user_config->drag_mode = true;  
  user_config->drag_time = 700;
  user_config->auto_trackpad_layer = true;
  user_config->scroll_threshold_time = 0;
  eeconfig_update_user(user_config->raw); 
  DRV_pulse(53);
  return eeconfig_read_user();
}

void set_opts(user_config_t user_config) {
  is_tap_mode = 1;
  hf_mode = user_config.hf_mode;
  is_layer_hf = user_config.layer_hf;
  is_drag_mode = user_config.drag_mode;  
  drag_time = user_config.drag_time ? user_config.drag_time : 700;
  is_auto_trackpad_layer = user_config.auto_trackpad_layer;
  scroll_threshold_time = user_config.scroll_threshold_time;
  accel_speed = 1;
  auto_trackpad_layer = 4;
  change_auto_trackpad_layer = false;
}

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  if(!user_config.init) {
    user_config.raw = init_opts(&user_config);
  }
  set_opts(user_config);

  vial_tap_dance_entry_t td0 = { SCLL_UP, KC_NO, SCLL_DOWN, KC_NO, DOUBLE_TAP_TIME_MS };
  vial_tap_dance_entry_t td1 = { DRG_UP, KC_NO, DRG_DOWN, KC_NO, DOUBLE_TAP_TIME_MS };
  vial_tap_dance_entry_t td2 = { HF_UP, KC_NO, HF_DOWN, KC_NO, DOUBLE_TAP_TIME_MS };
  dynamic_keymap_set_tap_dance(0, &td0);
  dynamic_keymap_set_tap_dance(1, &td1);
  dynamic_keymap_set_tap_dance(2, &td2);
}

void send_setting_string(char* t, int i) {
  char cn[12];
  sprintf(cn, "%d", i);
  char end = '\n';

  size_t len1 = strlen(t);
  size_t len2 = 1;
  size_t len3 = 1;
  size_t buffer_size = len1 + len2 + len3 + 1;

  char* c = (char*)malloc(buffer_size);
  memset(c, 0, buffer_size);

  strcat(c, t);
  strcat(c, cn);
  strncat(c, &end, 1);

  send_string(c);

  free(c);

}

void hf_DRV_pulse(bool ee2_up) {
  if (ee2_up) {
    user_config.hf_mode = hf_mode;  
    eeconfig_update_user(user_config.raw); 
    hf_mode = user_config.hf_mode;
  }
  DRV_pulse(hf_mode);
  send_setting_string("haptic: ", hf_mode);
}

void update_drag_time(uint32_t dt){
  user_config.drag_time = dt;
  eeconfig_update_user(user_config.raw); 
  drag_time = user_config.drag_time;
  send_setting_string("drag time: ", dt);
}

char* is_tap_mode_to_char(void) {
  return is_tap_mode ? "tap mode: on\n" : "tap mode: off\n";
}

char* is_layer_hf_to_char(void) {
  return user_config.layer_hf ? "haptic layer: on\n" : "haptic layer: off\n";
}

char* is_drag_mode_char(void) {
  return user_config.drag_mode ? "drag&drop mode: on\n" : "drag&drop mode: off\n";
}

char* is_auto_trackpad_layer_char(void) {
  return user_config.auto_trackpad_layer ? "auto trackpad layer mode: on\n" : "auto trackpad layer mode: off\n";
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case EE_CLR : 
      if (record->event.pressed) {
        user_config.raw = eeconfig_read_user();
        init_opts(&user_config);
        set_opts(user_config);
      }
      return false;     
    case KC_BTN1 ... KC_BTN5:
      if (record->event.pressed) {
        press_ms_btn = true;
      } else {
        press_ms_btn = false;
        if(change_auto_trackpad_layer) {
          layer_move(get_highest_layer(default_layer_state));
          change_auto_trackpad_layer = false;
        }
      }
      return false;   
    case KC_ACL0:
      if (record->event.pressed) {
        float as = accel_speed == 2 ? 1 : 2;
        accel_speed = as;
      }
      return false;  
    case KC_ACL1:
      if (record->event.pressed) {
        float as = accel_speed == 4 ? 1 : 4;
        accel_speed = as;
      }
      return false;  
    case SCLL_UP: 
      if (record->event.pressed) {
        scroll_threshold_time = scroll_threshold_time + 5;
        if(scroll_threshold_time > 100) {
          scroll_threshold_time = 100;
        }
        user_config.scroll_threshold_time = scroll_threshold_time;  
        eeconfig_update_user(user_config.raw); 
        send_setting_string("scroll time: ", scroll_threshold_time);
      }
      return false;
    case SCLL_DOWN: 
      if (record->event.pressed) {
        if(scroll_threshold_time != 0) {
          scroll_threshold_time = scroll_threshold_time - 5;
          user_config.scroll_threshold_time = scroll_threshold_time;  
          eeconfig_update_user(user_config.raw); 
        }
        send_setting_string("scroll time: ", scroll_threshold_time);      
      }
      return false;
    case KC_F19: 
      if (record->event.pressed) {
        user_config.tap_mode = !is_tap_mode;  
        eeconfig_update_user(user_config.raw); 
        is_tap_mode = user_config.tap_mode;    
        char* tm = is_tap_mode_to_char();
        send_string(tm);
      }
      return false;   
    case KC_F20: 
      if (record->event.pressed) {
        user_config.drag_mode = !is_drag_mode;  
        eeconfig_update_user(user_config.raw); 
        is_drag_mode = user_config.drag_mode;   
        char* dm = is_drag_mode_char();
        send_string(dm);
      }
      return false;             
    case DRG_UP: 
      if (record->event.pressed) {
        drag_time = drag_time + 10;
        if(drag_time > 3000) {
          drag_time = 3000;
        }
        update_drag_time(drag_time);
      }
      return false;
    case DRG_DOWN: 
      if (record->event.pressed) {
        if(drag_time != 0) {
          drag_time = drag_time - 10;
        }
        update_drag_time(drag_time);
      }
      return false;
    case KC_F21: 
      if (record->event.pressed) {
        user_config.layer_hf = !is_layer_hf;  
        eeconfig_update_user(user_config.raw); 
        is_layer_hf = user_config.layer_hf;  
        char* hl = is_layer_hf_to_char();  
        send_string(hl);
      }
      return false;         
    case HF_UP:
      if (record->event.pressed) {
        hf_mode++;
        if(hf_mode == 124) {
          hf_mode = 0;
        }
        hf_DRV_pulse(true);
      }
      return false;
    case HF_DOWN:
      if (record->event.pressed) {
        hf_mode--;
        if(hf_mode == -1) {
          hf_mode = 123;
        }
        hf_DRV_pulse(true);
      }
      return false;
    case KC_F22: 
      if (record->event.pressed) {
        user_config.auto_trackpad_layer = !is_auto_trackpad_layer;  
        eeconfig_update_user(user_config.raw); 
        is_auto_trackpad_layer = user_config.auto_trackpad_layer;  
        char* atl = is_auto_trackpad_layer_char();
        send_string(atl);
      }
      return false;    
    case KC_F23: 
      if (record->event.pressed) {  
        char st[100];
        sprintf(st, "scroll time: %d\n", user_config.scroll_threshold_time);
        char* tm = is_tap_mode_to_char();

        char* dm = is_drag_mode_char();
        char dt[100];
        sprintf(dt, "drag&drop time: %d\n", user_config.drag_time);

        char hm[100];
        sprintf(hm, "haptic number: %d\n", user_config.hf_mode);
        char* hl = is_layer_hf_to_char();

        char* atl = is_auto_trackpad_layer_char();
        char at[100];
        sprintf(at, "accel speed: %d\n", (int)accel_speed);
       
        size_t len1 = strlen(st);
        size_t len2 = strlen(tm);
        size_t len3 = strlen(dm);
        size_t len4 = strlen(dt);
        size_t len5 = strlen(hm);
        size_t len6 = strlen(hl);

        size_t len7 = strlen(atl);
        size_t len8 = strlen(at);

        size_t buffer_size = len1 + len2 + len3 + len4 + len5 + len6 + len7 + len8 + 1;
        char* c = (char*)malloc(buffer_size);
        memset(c, 0, buffer_size);

        strcat(c, st);
        strcat(c, tm);
        strcat(c, dm);
        strcat(c, dt);
        strcat(c, hm);
        strcat(c, hl);       
        strcat(c, atl);
        strcat(c, at);
       
        send_string(c);

        free(c);
      }
      return false; 
    default:
      return true;
  }
};

int layer = 0;

void matrix_scan_user(void) {
  int current_layer = get_highest_layer(layer_state|default_layer_state); 
  if(current_layer != auto_trackpad_layer && is_layer_hf && layer != current_layer){
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
    hsv.h = 222; //ROSE
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