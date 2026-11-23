/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include <stdint.h>
#include <stdbool.h>

/* TM1637 pin definitions (RP2040 CoreA) */
#ifndef TM1637_CLK_PIN
#    define TM1637_CLK_PIN GP10
#endif
#ifndef TM1637_DIO_PIN
#    define TM1637_DIO_PIN GP9
#endif

/* TM1637 commands */
#define TM1637_CMD_DATA      0x40  /* Data command: auto address increment, normal mode */
#define TM1637_CMD_DATA_FIXED 0x44 /* Data command: fixed address mode */
#define TM1637_CMD_ADDR      0xC0  /* Address command: set display address (OR with 0x00-0x05) */
#define TM1637_CMD_CTRL      0x80  /* Display control: OR with brightness (0x00-0x07), bit 3 = display on */
#define TM1637_DISPLAY_ON    0x08  /* Display on flag for control command */

/* Brightness levels (0-7) */
#define TM1637_BRIGHTNESS_MIN 0
#define TM1637_BRIGHTNESS_MAX 7
#define TM1637_BRIGHTNESS_DEFAULT 4

/* Special display patterns */
#define TM1637_SEG_BLANK 0x00
#define TM1637_SEG_DASH  0x40  /* Segment G only */

/* 7-segment digit encoding table (0-9)
 *    A
 *   ---
 * F|   |B
 *   -G-
 * E|   |C
 *   ---
 *    D   .DP
 *
 * Bit mapping: DP-G-F-E-D-C-B-A (bit7..bit0)
 */
extern const uint8_t tm1637_digit_table[10];

/**
 * Initialize TM1637 display.
 * Configures CLK/DIO pins and sets default brightness.
 */
void tm1637_init(void);

/**
 * Display a two-digit decimal number (0-99).
 * Left digit = tens, right digit = ones.
 * If number < 10, left digit is blank.
 */
void tm1637_display_number(uint8_t number);

/**
 * Display raw segment data on a specific digit position.
 * @param position  0 = first grid (GRID1), 1 = second grid (GRID2)
 * @param segments  Raw segment bit pattern
 */
void tm1637_display_raw(uint8_t position, uint8_t segments);

/**
 * Set display brightness.
 * @param brightness  0-7 (0=dimmest, 7=brightest)
 * @param on          true=display on, false=display off
 */
void tm1637_set_brightness(uint8_t brightness, bool on);

/**
 * Clear all digits (blank display).
 */
void tm1637_clear(void);
