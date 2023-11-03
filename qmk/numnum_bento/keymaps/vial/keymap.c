/* SPDX-License-Identifier: GPL-2.0-or-later */

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "iqs5xx.h"
#include "drivers/haptic/drv2605l.h"
#include <math.h>

typedef union {
  uint32_t raw;
  struct {
    bool init : 1;
    unsigned int hf_waveform_number : 7;
    bool can_hf_for_layer : 1;
    bool can_drag : 1;
    unsigned int scroll_term : 5;
    unsigned int drag_term : 4;
    bool can_trackpad_layer : 1;
    bool can_send_string : 1;
    bool drag_strength_mode : 1;
    unsigned int drag_strength : 4;
    unsigned int default_speed : 4;
  };
} user_config_t;
user_config_t user_config;

#define DOUBLE_KEY_TAP_TERM 200

int last_layer = 6;

enum {
  U_DRG_TOGG = QK_KB_0,
  U_DRG_MODE,
  U_HPL_TOGG,
  U_TPL_TOGG,
  U_SEND_STR_TOGG,
  U_SEND_SETTING,
  U_RESET_SETTING,
  U_S_STP1,
  U_S_STP2,
  U_S_STP3,
  U_M_ACL0,
  U_M_ACL1,
  U_M_ACL2,
  U_Layer_UP,
  U_Layer_DOWN,
  DRG_UP,
  DRG_DOWN,
  DRG_STRN_UP,
  DRG_STRN_DOWN,
  SCLL_UP,
  SCLL_DOWN,
  HF_UP,
  HF_DOWN,
  D_M_SPD_UP,
  D_M_SPD_DOWN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_BTN1, KC_BTN1, KC_BTN1, KC_BTN2        
  ),
  [1] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [2] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [3] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [4] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [5] = LAYOUT(
    KC_1,    KC_2,    KC_3,    KC_4,   KC_5,
    KC_6,    KC_7,    KC_8,    KC_9,   KC_0,
    KC_BTN1,    KC_BTN2,    U_S_STP1, U_M_ACL0, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    LGUI(KC_TAB),   LGUI(KC_TAB),
    LALT(KC_RGHT),  LCTL(LGUI(KC_RIGHT)), LALT(KC_LEFT), LCTL(LGUI(KC_LEFT)),
    LCTL(KC_PPLS),  LCTL(KC_PMNS),
    KC_ESC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
  [6] = LAYOUT(
    U_DRG_TOGG,    U_DRG_MODE,   TD(0),   TD(1),  U_SEND_STR_TOGG,
    U_HPL_TOGG, TD(2),    TD(3), TD(4), U_SEND_SETTING,
    KC_BTN1,    KC_BTN2,    QK_BOOT, U_RESET_SETTING, 

    KC_UP,    KC_LEFT,    KC_DOWN,    KC_RIGHT, U_Layer_UP,
    
    KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS,
    KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  )
};

uint32_t init_opts(user_config_t* user_config) {
  eeconfig_init_user();
  user_config->init = true;  
  user_config->can_hf_for_layer = true;
  user_config->hf_waveform_number = 47;   
  user_config->can_drag = true;  
  user_config->drag_term = 5;
  user_config->drag_strength_mode = true;
  user_config->drag_strength = 6;
  //user_config->can_trackpad_layer = true;
  user_config->scroll_term = 10;
  user_config->can_send_string = 1;
  user_config->default_speed = 9;
  eeconfig_update_user(user_config->raw); 
  drv2605l_pulse(53);
  return eeconfig_read_user();
}

void set_opts(user_config_t user_config) {
  hf_waveform_number = user_config.hf_waveform_number;
  can_hf_for_layer = user_config.can_hf_for_layer;
  can_drag = user_config.can_drag;  
  drag_term = user_config.drag_term * 100;
  can_trackpad_layer = false;
  scroll_term = user_config.scroll_term * 5;
  can_send_string = user_config.can_send_string;
  drag_strength_mode = user_config.drag_strength_mode;
  drag_strength = user_config.drag_strength + 1;
  default_speed = ((float)user_config.default_speed / 10) + 0.1;
  accel_speed = 1;
  scroll_step = 1;
  trackpad_layer = 5;
  use_trackpad_layer = false;
  use_drag = false;
}

char prefix_drag_term[] = "Drag&Drop Term: ";
char prefix_drag_strength[] = "Drag&Drop Strength: ";
char prefix_haptic_number[] = "HF Waveform Number: ";
char prefix_scroll_term[] = "Scroll Term: ";
char prefix_default_speed[] = "Default Speed: ";

char* can_hf_for_layer_to_char(void) {
  return can_hf_for_layer ? "HF for Layer: on\n" : "HF for Layer: off\n";
}

char* can_drag_char(void) {
  return can_drag ? "Drag&Drop: on\n" : "Drag&Drop: off\n";
}

char* drag_strength_mode_char(void) {
  return drag_strength_mode  ? "Drag&Drop Mode: Strength\n" : "Drag&Drop Mode: Term\n";
}

char* can_trackpad_layer_char(void) {
  return can_trackpad_layer ? "Trackpad Layer: on\n" : "Trackpad Layer: off\n";
}

char* can_send_string_char(void) {
  return can_send_string ? "Send String: on\n" : "Send String: off\n";
}

char *float_to_char(float f, int decimals) {
  int integer_part = (int)f;
  float fractional_part = f - integer_part;
    
  int scale = 1;
  for (int i = 0; i < decimals; i++) {
      scale *= 10;
      fractional_part *= 10;
  }
    
  int int_fractional_part = (int)(fractional_part + 0.5);
  char *buffer = (char *)malloc(20);
  snprintf(buffer, 20, "%d.%d", integer_part, int_fractional_part);
  return buffer;
}

void send_setting_string(char* t, char* cn) {
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

void send_setting_float_string(char* t, float f) {
  if(can_send_string){
    char *buffer = float_to_char(f, 1);
    send_setting_string(t, buffer);  
  }
}

void send_setting_int_string(char* t, int i) {
  if(can_send_string){
    char buffer[12];
    sprintf(buffer, "%d", i);
    send_setting_string(t, buffer);
  } 
}

void update_config_send_float_string(char* t, float f){
  eeconfig_update_user(user_config.raw); 
  send_setting_float_string(t, f);
}

void update_config_send_string(char* t, uint32_t i){
  eeconfig_update_user(user_config.raw); 
  send_setting_int_string(t, i);
}

void update_drag_term(uint32_t i){
  user_config.drag_term = i / 10;
  drag_term = i;
  update_config_send_string(prefix_drag_term, i);
}

void update_drag_strength(uint32_t i){
  user_config.drag_strength = i - 1;
  drag_strength = i;
  update_config_send_string(prefix_drag_strength, i);
}

void update_scroll_term(uint32_t i){
  user_config.scroll_term = i / 5;
  scroll_term = i;
  update_config_send_string(prefix_scroll_term, i);
}

void update_hf_waveform(uint32_t i){
  hf_waveform_number = user_config.hf_waveform_number = i;  
  update_config_send_string(prefix_haptic_number, i);
  drv2605l_pulse(hf_waveform_number);
} 

void update_default_speed(float f){
  user_config.default_speed = (f - 0.1) * 10;
  update_config_send_float_string(prefix_default_speed, f);
}

void layer_up(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == last_layer) {
    layer_clear();
  } else {
    layer_move(layer + 1); 
  }
} 

void layer_down(void) { 
  int layer = get_highest_layer(layer_state|default_layer_state);
  if (layer == 0) {
    layer_move(last_layer);
  } else {
    layer_move(layer - 1); 
  }
}  

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case U_Layer_UP:
      if (record->event.pressed) {    
        layer_up();
      }
      return false;   
    case U_Layer_DOWN:
      if (record->event.pressed) {    
        layer_down();
      }
      return false;   
    case U_DRG_TOGG: 
      if (record->event.pressed) {
        user_config.can_drag = !can_drag;  
        eeconfig_update_user(user_config.raw); 
        can_drag = user_config.can_drag;   
        char* dm = can_drag_char();
        if(can_send_string) {
          send_string(dm);
        }
      }
      return false; 
    case U_DRG_MODE: 
      if (record->event.pressed) {
        user_config.drag_strength_mode = !drag_strength_mode;
        eeconfig_update_user(user_config.raw); 
        drag_strength_mode = user_config.drag_strength_mode;   
        char* dm = drag_strength_mode_char();
        if(can_send_string) {
          send_string(dm);
        }
      }
      return false; 
    case U_HPL_TOGG: 
      if (record->event.pressed) {
        user_config.can_hf_for_layer = !can_hf_for_layer;  
        eeconfig_update_user(user_config.raw); 
        can_hf_for_layer = user_config.can_hf_for_layer;  
        char* hl = can_hf_for_layer_to_char(); 
        if(can_send_string) {
          send_string(hl);
        }
      }
      return false; 
    case U_TPL_TOGG: 
      if (record->event.pressed) {
        user_config.can_trackpad_layer = !can_trackpad_layer;  
        eeconfig_update_user(user_config.raw); 
        can_trackpad_layer = user_config.can_trackpad_layer;  
        char* atl = can_trackpad_layer_char();
        if(can_send_string) {
          send_string(atl);
        }
      }
      return false; 

    case U_SEND_STR_TOGG: 
      if (record->event.pressed) {
        user_config.can_send_string = !can_send_string;  
        eeconfig_update_user(user_config.raw); 
        can_send_string = user_config.can_send_string; 
        char* ss = can_send_string_char();
        if(can_send_string) {
          send_string(ss);
        }
      }
      return false;  
    case U_SEND_SETTING: 
      if (record->event.pressed) {  

        char* cd = can_drag_char();
        char* dm = drag_strength_mode_char();

        char dt[100];
        strcpy(dt, prefix_drag_term);     
        sprintf(dt + strlen(dt), "%d\n", (int)drag_term);

        char ds[100];
        strcpy(ds, prefix_drag_strength);     
        sprintf(ds + strlen(ds), "%d\n", (int)drag_strength);

        char* ch = can_hf_for_layer_to_char();

        char hm[100];
        strcpy(hm, prefix_haptic_number);     
        sprintf(hm + strlen(hm), "%d\n", (int)hf_waveform_number);

        //char* ct = can_trackpad_layer_char();
        
        char st[100];        
        strcpy(st, prefix_scroll_term); 
        sprintf(st + strlen(st), "%d\n", (int)scroll_term);

        char sst[100];
        sprintf(sst, "Scroll Step: %d\n", (int)scroll_step);

        char dms[100];        
        strcpy(dms, prefix_default_speed); 
        sprintf(dms + strlen(dms), "%s\n", float_to_char(default_speed, 1));

        char as[100];
        sprintf(as, "Accel Speed: %s\n", float_to_char(accel_speed, 1));

        char* ss = can_send_string_char();
        
        size_t len1 = strlen(cd);
        size_t len2 = strlen(dm);
        size_t len3 = strlen(dt);
        size_t len4 = strlen(ds);
        size_t len5 = strlen(ch);
        size_t len6 = strlen(hm);
        //size_t len7 = strlen(ct);
        size_t len8 = strlen(st);
        size_t len9 = strlen(sst);
        size_t len10 = strlen(dms);       
        size_t len11 = strlen(as);
        size_t len12 = strlen(ss);

        size_t buffer_size = len1 + len2 + len3 + len4 + len5 + len6 + len8 + len9 + len10 + len11 + len12 + 1;
        char* c = (char*)malloc(buffer_size);
        memset(c, 0, buffer_size);

        strcat(c, cd);
        strcat(c, dm);
        strcat(c, dt);
        strcat(c, ds);      
        strcat(c, ch);       
        strcat(c, hm);
        //strcat(c, ct);
        strcat(c, st);
        strcat(c, sst);
        strcat(c, dms);
        strcat(c, as);
        strcat(c, ss);

        send_string(c);

        free(c);
      }
      return false; 
    case U_RESET_SETTING : 
      if (record->event.pressed) {
        user_config.raw = eeconfig_read_user();
        init_opts(&user_config);
        set_opts(user_config);
      }
      return false;
    case U_S_STP1:
      if (record->event.pressed) {
        scroll_step = scroll_step == 4 ? 1 : 4;
      }
      return false;  
    case U_S_STP2:
      if (record->event.pressed) {
        scroll_step = scroll_step == 8 ? 1 : 8;
      }
      return false; 
    case U_S_STP3:
      if (record->event.pressed) {
        scroll_step = scroll_step == 16 ? 1 : 16;
      }
      return false; 
    case U_M_ACL0:
      if (record->event.pressed) {
        float epsilon = 1e-6f;
        if (fabs(accel_speed - 0.5f) < epsilon) {
          accel_speed = 1.0f;
        } else {
          accel_speed = 0.5f;
        }
      }
      return false;  
    case U_M_ACL1:
      if (record->event.pressed) {
        accel_speed = accel_speed == 2 ? 1 : 2;
      }
      return false;  
    case U_M_ACL2:
      if (record->event.pressed) {
        accel_speed = accel_speed == 4 ? 1 : 4;
      }
      return false;  
    case DRG_UP: 
      if (record->event.pressed) {
        if(drag_term < 1000) {
          drag_term = drag_term + 100;
          update_drag_term(drag_term);
        } else {
          send_setting_int_string(prefix_drag_term, drag_term);
        }
      }
      return false;
    case DRG_DOWN: 
      if (record->event.pressed) {
        if(drag_term > 0) {
          drag_term = drag_term - 100;
          update_drag_term(drag_term);
        } else {
          send_setting_int_string(prefix_drag_term, drag_term);
        }
      }
      return false;
    case DRG_STRN_UP: 
      if (record->event.pressed) {
        if(drag_strength < 16) {
          drag_strength = drag_strength + 1;
          update_drag_strength(drag_strength);
        } else {
          send_setting_int_string(prefix_drag_strength, drag_strength);
        }
      }
      return false;
    case DRG_STRN_DOWN: 
      if (record->event.pressed) {
        if(drag_strength > 1) {
          drag_strength = drag_strength - 1;
          update_drag_strength(drag_strength);
        } else {
          send_setting_int_string(prefix_drag_strength, drag_strength);
        }
      }
      return false;
    case SCLL_UP: 
      if (record->event.pressed) {
        if(scroll_term < 100) {
          scroll_term = scroll_term + 5;
          update_scroll_term(scroll_term);
        } else {
          send_setting_int_string(prefix_scroll_term, scroll_term);
        } 
      }
      return false;
    case SCLL_DOWN: 
      if (record->event.pressed) {
        if(scroll_term > 0) {
          scroll_term = scroll_term - 5;
          update_scroll_term(scroll_term);
        } else {
          send_setting_int_string(prefix_scroll_term, scroll_term);
        }   
      }
      return false;         
    case HF_UP:
      if (record->event.pressed) {
        hf_waveform_number++;
        if(hf_waveform_number == 124) {
          hf_waveform_number = 0;
        }
        update_hf_waveform(hf_waveform_number);
      }
      return false;
    case HF_DOWN:
      if (record->event.pressed) {
        hf_waveform_number--;
        if(hf_waveform_number == -1) {
          hf_waveform_number = 123;
        }
        update_hf_waveform(hf_waveform_number);
      }
      return false;
    case D_M_SPD_UP: 
      if (record->event.pressed) {
        if(default_speed < 1.5) {
          default_speed = default_speed + 0.1;
          update_default_speed(default_speed);
        } else {
          send_setting_float_string(prefix_default_speed, default_speed);
        }
      }
      return false;
    case D_M_SPD_DOWN: 
      if (record->event.pressed) {
        if(default_speed > 0.1) {
          default_speed = default_speed - 0.1;
          update_default_speed(default_speed);
        } else {
          send_setting_float_string(prefix_default_speed, default_speed);
        }
      }
      return false;
    default:
      return true;
  }
};

//haptic
bool get_haptic_enabled_key(uint16_t keycode, keyrecord_t *record) {
    return false;
}

void update_hf_setting(bool is_eep) {
  if (is_eep) {
    user_config.hf_waveform_number = hf_waveform_number;  
    eeconfig_update_user(user_config.raw); 
    hf_waveform_number = user_config.hf_waveform_number;
  }
  drv2605l_pulse(hf_waveform_number);
  send_setting_int_string(prefix_haptic_number, hf_waveform_number);
}

//funkyswtich
bool center_push_holding = false;
keypos_t center_push = {.row = 2, .col = 4}; 

bool in_center_push_press(void) {
  return matrix_is_on(0, 4) &&
    !matrix_is_on(0, 0) && 
    !matrix_is_on(0, 1) && 
    !matrix_is_on(0, 2) && 
    !matrix_is_on(0, 3);
}

void center_push_press(keypos_t k, bool *center_push_holding, bool in_center_push_press) {
  keyevent_t k_event = { .key = k, .type = KEY_EVENT };

  if(!*center_push_holding && in_center_push_press){
    k_event.pressed = true;
    k_event.time = timer_read();
    action_exec(k_event);   
    *center_push_holding = true;
  } else if(*center_push_holding && !in_center_push_press){ 
    k_event.pressed = false;
    k_event.time = timer_read();
    action_exec(k_event);
    *center_push_holding = false;
  }
}

//kb
void keyboard_post_init_user(void) {
   user_config.raw = eeconfig_read_user();
  if(!user_config.init) {
    user_config.raw = init_opts(&user_config);
  }
  set_opts(user_config);

  vial_tap_dance_entry_t td0 = { DRG_UP, KC_NO, DRG_DOWN, KC_NO, DOUBLE_KEY_TAP_TERM };
  vial_tap_dance_entry_t td1 = { DRG_STRN_UP, KC_NO, DRG_STRN_DOWN, KC_NO, DOUBLE_KEY_TAP_TERM };
  vial_tap_dance_entry_t td2 = { HF_UP, KC_NO, HF_DOWN, KC_NO, DOUBLE_KEY_TAP_TERM };
  vial_tap_dance_entry_t td3 = { SCLL_UP, KC_NO, SCLL_DOWN, KC_NO,  DOUBLE_KEY_TAP_TERM };
  vial_tap_dance_entry_t td4 = { D_M_SPD_UP, KC_NO, D_M_SPD_DOWN, KC_NO,  DOUBLE_KEY_TAP_TERM };

  dynamic_keymap_set_tap_dance(0, &td0);
  dynamic_keymap_set_tap_dance(1, &td1);
  dynamic_keymap_set_tap_dance(2, &td2);
  dynamic_keymap_set_tap_dance(3, &td3);
  dynamic_keymap_set_tap_dance(4, &td4);
}

int layer;
void matrix_scan_user(void) {
  center_push_press(center_push, &center_push_holding, in_center_push_press());
  int current_layer = get_highest_layer(layer_state|default_layer_state); 
  if(can_hf_for_layer && layer != current_layer){
    drv2605l_pulse(hf_waveform_number);
    layer = current_layer;
  }
}

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [1] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [2] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [3] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [4] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [5] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) },
    [6] = { ENCODER_CCW_CW(C(KC_PMNS), C(KC_PPLS)) }
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int current_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (current_layer == 1) {
      hsv.h = 213; //MAGENTA
    } else if (current_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (current_layer == 3)  {
      hsv.h = 170; //BLUE
    } else if (current_layer == 4)  {
      hsv.h = 11; //CORAL
      hsv.s = 176;
    } else if (current_layer == 5)  {
      hsv.h = 43; //YELLOW
    } else if (current_layer == 6)  {
      hsv.h = 255; //RED
    } else {
      hsv.h = 128; //CYAN
    }
    RGB rgb = hsv_to_rgb(hsv);
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
    return false;
};