#include QMK_KEYBOARD_H
#include "uzu42.h"
#include "lufa.h"
#include "split_util.h"
#include "ssd1306.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( \
  //,---------------------------------------.                  ,---------------------------------------.
      KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                      KC_Y,   KC_U,   KC_I,   KC_O,   KC_P,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
MT(MOD_LCTL,KC_A),   KC_S,   KC_D,   KC_F,   KC_G,               KC_H,   KC_J,   KC_K,   KC_L, KC_BSLS,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
MT(MOD_LSFT,KC_Z),   KC_X,   KC_C,   KC_V,   KC_B,               KC_N,   KC_M,KC_COMM, KC_DOT,KC_SLSH,\
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
      KC_ESC, KC_TAB,KC_LALT,MO(2), MO(1),   KC_SPC,    KC_ENT,  KC_BSPC, MO(1),  MO(2), KC_RCTL,KC_RSFT \
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
  ),

  [1] = LAYOUT( \
  //,---------------------------------------.                  ,---------------------------------------.
     S(KC_1), S(KC_2), S(KC_3), S(KC_4), S(KC_5),     S(KC_6), S(KC_7), S(KC_8), S(KC_9), S(KC_0),  \
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
     KC_GRV,S(KC_GRV),_______,_______,_______,                   KC_LEFT,KC_DOWN,  KC_UP,KC_RGHT, _______,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
     _______, _______,_______,_______, _______,                 _______,KC_MINS, KC_EQL,S(KC_MINS), S(KC_EQL),\
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
     KC_LGUI,_______,_______,MO(3),_______,_______,     _______,KC_DEL, KC_LBRC, KC_RBRC,KC_SCLN, KC_QUOT\
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
  ),

  [2] = LAYOUT( \
  //,---------------------------------------.                  ,---------------------------------------.
        KC_1,   KC_2,   KC_3,   KC_4,   KC_5,                      KC_6,   KC_7,   KC_8,   KC_9,   KC_0,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
      _______,_______,_______,_______,_______,                   _______,_______,  _______,_______,_______,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
     _______,_______, _______,_______, _______,                   _______,_______,  _______,_______,_______,\
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
     _______,_______,_______,_______,_______,_______,   _______,_______,_______,_______,_______,_______ \
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
  ),

  [3] = LAYOUT( \
  //,---------------------------------------.                  ,---------------------------------------.
       KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,                     KC_F6,  KC_F7,  KC_F8,  KC_F9, KC_F10,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
      KC_F11, KC_F12,  RESET,KC_PSCR, KC_INS,                   RGB_TOG,RGB_HUI,RGB_SAI,RGB_VAI,XXXXXXX,\
  //|-------+-------+-------+-------+-------|                  |-------+-------+-------+-------+-------|
     XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,                   RGB_MOD,RGB_HUD,RGB_SAD,RGB_VAD,XXXXXXX,\
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
     _______,_______,_______,_______,_______,_______,   _______,_______,_______,_______,_______,_______ \
  //|-------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------|
  )
};


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

const char *read_logo(void) {
  static char logo[] = {
      0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
      0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
      0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
      0};
  return logo;
}

void render_rgb_status(void) {

    switch(rgb_matrix_config.mode) {
            case 0:
                oled_write_ln_P(PSTR("RGB: NONE"), false);
                break;
            case 1:
                oled_write_ln_P(PSTR("RGB: SOLID_COLOR"), false);
                break;
            case 2:
                oled_write_ln_P(PSTR("RGB: BREATHING"), false);
                break;
            case 3:
                oled_write_ln_P(PSTR("RGB: CYCLE_ALL"), false);
                break;
            case 4:
                oled_write_ln_P(PSTR("RGB: RAINBOW_BEACON"), false);
                  break;                  
            case 5:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_SMPL"), false);
                  break;
            case 6:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT"), false);
                  break;
            case 7:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_WIDE"), false);
                  break;
            case 8:
                  oled_write_ln_P(PSTR("RGB: SLD_REACT_CROSS"), false);
                  break;
            case 9:
                  oled_write_ln_P(PSTR("RGB: SPLASH"), false);
                  break;      
            default:
                oled_write_ln_P(PSTR("RGB: OTHER"), false);
                break;
    }
}

void oled_task_user(void) {
  if (is_keyboard_master()) {
    char layer[12];    

    snprintf(layer, sizeof(layer), "Layer: %d", get_highest_layer(layer_state));

    oled_write_ln_P(PSTR("UZU42"), false);
    oled_write_ln(layer, false);
    render_rgb_status();
  } else {
    oled_write(read_logo(), false);
  }
}

led_config_t g_led_config = { {
  // Key Matrix to LED Index
  { 26, 19, 18, 11, 10 },
  { 25, 20, 17, 12, 9 },
  { 24, 21, 16, 13, 8 }, 
  { 23, 22, 15, 14, 7, 6 },
  { 53, 46, 45, 38, 37 }, 
  { 52, 47, 44, 39, 36 }, 
  { 51, 48, 43, 40, 35 }, 
  { 50, 49, 42, 41, 34, 33 }  
}, {
  // LED Index to Physical Position 
  {0, 0},  {0, 0},{0, 0},{0, 0},{0, 0}, //ignore
  {0, 0},  {25, 0},{50, 0},  {75, 0},  {100, 0},  
  {0, 21},{25, 21}, {50, 21},{75, 21},  {100, 21}, 
  {0, 42},  {25, 42},  {50, 42}, {75, 42},  {100, 42},
  {0, 63}, {20, 63},{40, 63},  {60, 63},  {80, 63}, {100, 63},
  {0, 0},  {0, 0},{0, 0},{0, 0},{0, 0},  //ignore
  {125, 0},  {150, 0},    {175, 0},     {200, 0},   {220, 0}, 
  {125, 21},   {150, 21},   {175, 21},     {200, 21},   {220, 21}, 
  {125, 42},   {150, 42},   {175, 42},     {200, 42},   {220, 42}, 
  {120, 63},  {140, 63},  {160, 63},   {180, 63},  {200, 63},  {220, 63},  
} ,{
  // LED Index to Flag    
  2, 2, 2, 2, 2, 2,
  4, 4, 4, 4, 4,        
  4, 4, 4, 4, 4, 
  4, 4, 4, 4, 4,        
  4, 4, 4, 4, 4, 4,

  2, 2, 2, 2, 2, 2,      
  4, 4, 4, 4, 4,       
  4, 4, 4, 4, 4, 
  4, 4, 4, 4, 4, 
  4, 4, 4, 4, 4, 4,  
} 
};

void rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) { 
    int is_layer = get_highest_layer(layer_state|default_layer_state);  
    HSV hsv = {0, 255, rgblight_get_val()};
    if (is_layer == 1) {
      hsv.h = 128; //CYAN
    } else if (is_layer == 2)  {
      hsv.h = 85; //GREEN
    } else if (is_layer == 3)  {
      hsv.h = 43; //YELLOW
    } else {
      hsv.h = 191; //PURPLE
    }
    RGB rgb = hsv_to_rgb(hsv);
 
    for (uint8_t i = led_min; i <= led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
    }
};
