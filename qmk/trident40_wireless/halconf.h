// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define HAL_USE_SERIAL TRUE   // USART1: link to the HY0020
#define HAL_USE_ADC TRUE      // battery voltage

// One Vial frame is 36 bytes; the HY0020 update tunnel moves up to 30 bytes per report.
#define SERIAL_BUFFERS_SIZE 256

#include_next <halconf.h>
