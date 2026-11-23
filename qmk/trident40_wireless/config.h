// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// ---- HY0020 (nRF52832) -------------------------------------------------------------------
// USART1 PB6 (TX) -> P0.08, PB7 (RX) <- P0.06; AF7.
#define BT_UART_DRIVER SD1
#define BT_TX_PIN B6
#define BT_RX_PIN B7
#define BT_UART_PAL_MODE 7
#define BT_IRQ_PIN A1    // <- P0.07: low = the nRF is about to send (wakes the STM32 from Stop)
#define BT_NRST_PIN A2   // -> P0.21/nRESET: driven low only for a reset pulse
#define BT_DFU_PIN A3    // -> P0.20 (documented DFU pin; the bare HY0020 bootloader ignores it, left as a pulled-up input)

// ---- power -------------------------------------------------------------------------------
#define VBUS_DET_PIN B3  // VBUS / 2: high = USB cable plugged in
#define VBAT_ADC_PIN B1  // battery (CR2450) through R10, ADC1_IN16

// Sleep this long between matrix scans (the core waits in WFI instead of spinning).
#define LP_SCAN_INTERVAL_MS 1
// On battery: Stop 2 after this long with no key down and nothing to do. A key, the nRF's IRQ or
// a USB cable wakes it.
#define LP_STOP_IDLE_MS 30

// Battery: CR2450 through the boost converter. Percent is linear between these (a CR2450 stays
// near 3.0 V for most of its life and drops quickly at the end).
#define BATTERY_FULL_MV 3000
#define BATTERY_EMPTY_MV 2200
// On USB (the cell is not loaded): one resting sample this often (battery.h).
#define BATTERY_USB_SAMPLE_MS 60000

// Debounce 10 ms (QMK default 5): a doubled "y" on BLE (2026-11-06), trying a longer window first.
#define DEBOUNCE 10
