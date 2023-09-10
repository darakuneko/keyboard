/*
Copyright 2022 daraku_neko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define I2C1_SCL_PIN B8 
#define I2C1_SDA_PIN B9
#define I2C1_CLOCK_SPEED    400000
#define I2C1_DUTY_CYCLE     FAST_DUTY_CYCLE_2
#define OLED_DISPLAY_128X64
#define OLED_FONT_H "keymaps/default/glcdfont.c"

#define WS2812_PWM_DRIVER PWMD3
#define WS2812_PWM_CHANNEL 4
#define WS2812_PWM_PAL_MODE 2
#define WS2812_DMA_STREAM STM32_DMA1_STREAM2
#define WS2812_DMA_CHANNEL 5

#define RGB_MATRIX_LED_COUNT 84
#define RGB_MATRIX_CENTER { 112, 32 }
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 180
#define RGB_MATRIX_STARTUP_VAL 180
#define RGB_MATRIX_TIMEOUT 120000
#define RGB_DISABLE_WHEN_USB_SUSPENDED true
#define RGB_MATRIX_HUE_STEP 4
#define RGB_MATRIX_SAT_STEP 4
#define RGB_MATRIX_VAL_STEP 4
#define RGB_MATRIX_SPD_STEP 4
#define RGB_MATRIX_KEYPRESSES
#define RGB_MATRIX_FRAMEBUFFER_EFFECTS
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CYCLE_OUT_IN

#define VIAL_KEYBOARD_UID {0x57, 0x2C, 0xA4, 0x12, 0xA8, 0xF3, 0x29, 0xD5}
#define VIAL_UNLOCK_COMBO_ROWS { 0, 1 }
#define VIAL_UNLOCK_COMBO_COLS { 0, 0 }