// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Low power clocks (as trident42_lowpower): SYSCLK = HSI16 (16 MHz) straight, PLL off. USB keeps
// its own 48 MHz from HSI48, and USB FS needs HCLK >= 14.2 MHz, so 16 MHz is the floor. Waking from
// Stop restarts on HSI16 = the run clock, so only HSI48 has to be switched back on (power.c).

#pragma once

#include_next <mcuconf.h>

#undef STM32_SW
#define STM32_SW STM32_SW_HSI16

#undef STM32_STOPWUCK
#define STM32_STOPWUCK STM32_STOPWUCK_HSI16

#undef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1 TRUE

#undef STM32_ADC_USE_ADC1
#define STM32_ADC_USE_ADC1 TRUE
