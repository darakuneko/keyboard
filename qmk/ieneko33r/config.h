/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#define WS2812_PWM_DRIVER PWMD1
#define WS2812_PWM_CHANNEL 2
#define WS2812_PWM_PAL_MODE 1
#define WS2812_PWM_COMPLEMENTARY_OUTPUT
#define WS2812_DMA_STREAM STM32_DMA1_STREAM6
#define WS2812_DMA_CHANNEL 7 

#define VIAL_KEYBOARD_UID {0xC3, 0xE5, 0xEB, 0x45, 0x19, 0x19, 0x25, 0x0A}

#define VIAL_UNLOCK_COMBO_ROWS {0,0}
#define VIAL_UNLOCK_COMBO_COLS {0,1}
