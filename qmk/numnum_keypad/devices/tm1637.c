/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "tm1637.h"
#include "gpio.h"
#include "wait.h"

/* Bit-bang timing delay (microseconds).
 * 5 µs provides reliable communication with most TM1637 variants. */
#define TM1637_DELAY_US 5

/* --- Open-drain helpers for DIO ---
 * TM1637 uses an open-drain-like protocol on DIO.
 * HIGH: release the line (input with pull-up)
 * LOW:  drive the line low (output low)
 */
static void dio_high(void) {
    gpio_set_pin_input_high(TM1637_DIO_PIN);  /* input + internal pull-up */
}

static void dio_low(void) {
    gpio_write_pin_low(TM1637_DIO_PIN);
    gpio_set_pin_output(TM1637_DIO_PIN);
}

static void clk_high(void) {
    gpio_set_pin_input_high(TM1637_CLK_PIN);
}

static void clk_low(void) {
    gpio_write_pin_low(TM1637_CLK_PIN);
    gpio_set_pin_output(TM1637_CLK_PIN);
}

/* 7-segment encoding: DP-G-F-E-D-C-B-A */
const uint8_t tm1637_digit_table[10] = {
    0x3F, /* 0: A+B+C+D+E+F     */
    0x06, /* 1: B+C               */
    0x5B, /* 2: A+B+D+E+G         */
    0x4F, /* 3: A+B+C+D+G         */
    0x66, /* 4: B+C+F+G           */
    0x6D, /* 5: A+C+D+F+G         */
    0x7D, /* 6: A+C+D+E+F+G       */
    0x07, /* 7: A+B+C             */
    0x7F, /* 8: A+B+C+D+E+F+G     */
    0x6F, /* 9: A+B+C+D+F+G       */
};

static uint8_t tm1637_brightness = TM1637_BRIGHTNESS_DEFAULT;
static bool    tm1637_on        = true;

/* --- Low-level bit-bang protocol --- */

static void tm1637_start(void) {
    /* DIO: HIGH -> LOW while CLK is HIGH */
    dio_high();
    clk_high();
    wait_us(TM1637_DELAY_US);
    dio_low();
    wait_us(TM1637_DELAY_US);
    clk_low();
    wait_us(TM1637_DELAY_US);
}

static void tm1637_stop(void) {
    /* DIO: LOW -> HIGH while CLK is HIGH */
    dio_low();
    clk_low();
    wait_us(TM1637_DELAY_US);
    clk_high();
    wait_us(TM1637_DELAY_US);
    dio_high();
    wait_us(TM1637_DELAY_US);
}

static void tm1637_write_byte(uint8_t data) {
    /* Send 8 bits, LSB first */
    for (uint8_t i = 0; i < 8; i++) {
        clk_low();
        wait_us(TM1637_DELAY_US);

        if (data & (1 << i)) {
            dio_high();
        } else {
            dio_low();
        }
        wait_us(TM1637_DELAY_US);

        clk_high();
        wait_us(TM1637_DELAY_US);
    }

    /* ACK bit: release DIO, pulse CLK, read ACK */
    clk_low();
    dio_high();  /* release DIO for TM1637 to pull low as ACK */
    wait_us(TM1637_DELAY_US);

    clk_high();
    wait_us(TM1637_DELAY_US);

    clk_low();
    wait_us(TM1637_DELAY_US);
}

static void tm1637_send_command(uint8_t cmd) {
    tm1637_start();
    tm1637_write_byte(cmd);
    tm1637_stop();
}

/* --- Internal helpers --- */

/* Send the display control command respecting current on/off state. */
static void tm1637_refresh_control(void) {
    uint8_t ctrl = TM1637_CMD_CTRL | tm1637_brightness;
    if (tm1637_on) {
        ctrl |= TM1637_DISPLAY_ON;
    }
    tm1637_send_command(ctrl);
}

/* Write two segment bytes to GRID1 and GRID2 using auto-increment mode.
 * GRID1 (addr 0x00) = left digit, GRID2 (addr 0x01) = right digit. */
static void tm1637_write_two_digits(uint8_t left, uint8_t right) {
    tm1637_send_command(TM1637_CMD_DATA);

    tm1637_start();
    tm1637_write_byte(TM1637_CMD_ADDR | 0x00);
    tm1637_write_byte(left);
    tm1637_write_byte(right);
    tm1637_stop();

    tm1637_refresh_control();
}

/* --- Public API --- */

void tm1637_init(void) {
    /* Start with both lines released HIGH (open-drain idle) */
    clk_high();
    dio_high();
    wait_us(TM1637_DELAY_US * 4);  /* let lines settle */

    tm1637_set_brightness(TM1637_BRIGHTNESS_DEFAULT, true);
    tm1637_clear();
}

void tm1637_display_number(uint8_t number) {
    if (number > 99) number = 99;

    uint8_t tens = number / 10;
    uint8_t ones = number % 10;
    uint8_t left  = (tens > 0) ? tm1637_digit_table[tens] : TM1637_SEG_BLANK;
    uint8_t right = tm1637_digit_table[ones];

    tm1637_write_two_digits(left, right);
}

void tm1637_display_raw(uint8_t position, uint8_t segments) {
    tm1637_send_command(TM1637_CMD_DATA_FIXED);

    tm1637_start();
    tm1637_write_byte(TM1637_CMD_ADDR | (position & 0x01));
    tm1637_write_byte(segments);
    tm1637_stop();

    tm1637_refresh_control();
}

void tm1637_set_brightness(uint8_t brightness, bool on) {
    if (brightness > TM1637_BRIGHTNESS_MAX) {
        brightness = TM1637_BRIGHTNESS_MAX;
    }
    tm1637_brightness = brightness;
    tm1637_on = on;

    tm1637_refresh_control();
}

void tm1637_clear(void) {
    tm1637_write_two_digits(TM1637_SEG_BLANK, TM1637_SEG_BLANK);
}
