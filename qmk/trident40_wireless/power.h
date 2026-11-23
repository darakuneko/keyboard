// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdbool.h>
#include <stdint.h>

void     power_init(void);
void     power_task(void);      // end of every main loop pass: WFI wait, or Stop on battery
void     power_busy(void);      // something is going on: do not stop for LP_STOP_IDLE_MS
bool     power_vbus(void);      // USB cable plugged in
uint32_t power_wakeups(void);   // number of wake-ups from Stop so far

// Counters since the last reset, for the USB diagnostics command (hy0020_update.c).
typedef struct __attribute__((packed)) {
    uint32_t stops;          // wake-ups from Stop (the rest of them, after the three below: VBUS)
    uint32_t wake_key;
    uint32_t wake_irq;       // the nRF's IRQ
    uint32_t wake_other;     // none of those pins active on waking: anything else that ended the WFE
    uint32_t skip_key_held;  // Stop not entered: a row reads high with all columns driven
    uint32_t skip_busy;      // Stop not entered: VBUS, IRQ low or the link busy with the edges armed
    uint32_t awake_ms;       // time awake on battery (the timer stands still in Stop)
    uint8_t  reset_flags;    // RCC_CSR bits 31..24 of the last reset
} power_diag_t;
const power_diag_t *power_diag(void);
