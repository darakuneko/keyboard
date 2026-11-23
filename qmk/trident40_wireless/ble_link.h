// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Link to the HY0020 (nRF52832) on USART1; implements QMK's custom Bluetooth driver.

#pragma once

#include <stdbool.h>
#include <stdint.h>

uint8_t ble_link_state(void);                       // BT_STATE_* as last reported by the nRF
void    ble_link_command(uint8_t cmd);              // BT_CMD_*
void    ble_link_battery(uint8_t percent, uint16_t mv);
void    ble_link_ping(void);                        // the nRF answers with its state
void    ble_link_request_stats(void);               // the nRF answers with its power counters (MSG_STATS)
uint8_t ble_link_nrf_stats(uint8_t *out);           // last MSG_STATS payload (up to 30 bytes), its length (0: none yet)
uint8_t ble_link_nrf_fault(uint8_t *out);           // last MSG_FAULT payload (30 bytes); returns how many arrived since start
void    ble_link_wake_nrf(void);                    // a key on battery while not connected: let it reconnect

// Power (power.c): may the STM32 enter Stop now? False while a frame is half sent or received.
bool ble_link_idle(void);
void ble_link_slept(void);                          // back from Stop: the nRF may have stopped its UART

// Counters for the USB diagnostics command (hy0020_update.c).
typedef struct {
    uint8_t  state;          // BT_STATE_*
    uint8_t  running;        // link owns USART1 (not in update mode)
    uint8_t  hello[5];       // last HELLO payload
    uint16_t hellos;
    uint16_t rx_frames;
    uint16_t rx_crc_errors;
    uint16_t tx_frames;
    uint16_t nrf_rx_frames;  // the nRF's own counts, from its last STATUS
    uint16_t nrf_rx_dropped;
    uint16_t conn_interval;  // x 1.25 ms, from the nRF's last STATUS (0: not connected)
    uint16_t conn_latency;
    uint16_t conn_timeout;   // x 10 ms
} ble_link_diag_t;
const ble_link_diag_t *ble_link_diag(void);

// HY0020 update (hy0020_update.c) takes USART1 over and gives it back.
void ble_link_release(void);
void ble_link_resume(void);
