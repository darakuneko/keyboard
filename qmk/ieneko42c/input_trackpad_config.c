//If we're going to use it, let's organize it.
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
  eeconfig_update_user_datablock(&trackpad_config); 
  send_setting_float_string(t, f);
}

void update_config_send_string(char* t, uint32_t i){
  eeconfig_update_user_datablock(&trackpad_config); 
  send_setting_int_string(t, i);
}

void update_drag_term(uint32_t i){
  drag_term = trackpad_config.drag_term = i;
  update_config_send_string(prefix_drag_term, i);
}

void update_drag_strength(uint32_t i){
  trackpad_config.drag_strength = i;
  drag_strength = i;
  update_config_send_string(prefix_drag_strength, i);
}

void update_scroll_term(uint32_t i){
  scroll_term = trackpad_config.scroll_term = i;
  update_config_send_string(prefix_scroll_term, i);
}

void update_hf_waveform(uint32_t i){
  hf_waveform_number = trackpad_config.hf_waveform_number = i;  
  update_config_send_string(prefix_haptic_number, i);
  drv2605l_pulse(hf_waveform_number);
} 

void update_default_speed(float f){
  trackpad_config.default_speed = f * 10;
  update_config_send_float_string(prefix_default_speed, f);
}


bool update_input_trackpad_setting(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {  
    case U_DRG_TOGG: 
      if (record->event.pressed) {
        trackpad_config.can_drag = !can_drag;  
        eeconfig_update_user_datablock(&trackpad_config); 
        can_drag = trackpad_config.can_drag;   
        char* dm = can_drag_char();
        if(can_send_string) {
          send_string(dm);
        }
      }
      return false; 
    case U_DRG_MODE: 
      if (record->event.pressed) {
        trackpad_config.drag_strength_mode = !drag_strength_mode;
        eeconfig_update_user_datablock(&trackpad_config); 
        drag_strength_mode = trackpad_config.drag_strength_mode;   
        char* dm = drag_strength_mode_char();
        if(can_send_string) {
          send_string(dm);
        }
      }
      return false; 
    case U_HPL_TOGG: 
      if (record->event.pressed) {
        trackpad_config.can_hf_for_layer = !can_hf_for_layer;  
        eeconfig_update_user_datablock(&trackpad_config); 
        can_hf_for_layer = trackpad_config.can_hf_for_layer;  
        char* hl = can_hf_for_layer_to_char(); 
        if(can_send_string) {
          send_string(hl);
        }
      }
      return false; 
    case U_TPL_TOGG: 
      if (record->event.pressed) {
        trackpad_config.can_trackpad_layer = !can_trackpad_layer;  
        eeconfig_update_user_datablock(&trackpad_config); 
        can_trackpad_layer = trackpad_config.can_trackpad_layer;  
        char* atl = can_trackpad_layer_char();
        if(can_send_string) {
          send_string(atl);
        }
      }
      return false; 

    case U_SEND_STR_TOGG: 
      if (record->event.pressed) {
        trackpad_config.can_send_string = !can_send_string;  
        eeconfig_update_user_datablock(&trackpad_config); 
        can_send_string = trackpad_config.can_send_string; 
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
        eeconfig_read_user_datablock(&trackpad_config);
        init_opts(&trackpad_config);
        set_opts(trackpad_config);
      }
      return false;
    case DRG_UP: 
      if (record->event.pressed) {
        if(drag_term < 1000) {
          drag_term = drag_term + 10;;
          update_drag_term(drag_term);
        } else {
          send_setting_int_string(prefix_drag_term, drag_term);
        }
      }
      return false;
    case DRG_DOWN: 
      if (record->event.pressed) {
        if(drag_term > 0) {
          drag_term = drag_term - 10;
          update_drag_term(drag_term);
        } else {
          send_setting_int_string(prefix_drag_term, drag_term);
        }
      }
      return false;
    case DRG_STRN_UP: 
      if (record->event.pressed) {
        if(drag_strength < 20) {
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
        if(scroll_term < 1000) {
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
        if(default_speed < 5.0) {
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

void set_keyboard_post_init_user(void) {
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
