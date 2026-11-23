// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Power. 16 MHz core (mcuconf.h) and WFI between matrix scans always. On battery (no VBUS) the
// STM32 also stops (Stop 2, ~1 uA) whenever no key is down and nothing is going on. It wakes on:
//   - a key: all columns driven high, the rows pulled down, a rising edge on any row. The row
//     pins have distinct EXTI lines (PA6, PA5, PA7, PB4 = lines 6, 5, 7, 4); the columns do not.
//   - the nRF's IRQ (PA1, falling): it is about to send a frame.
//   - VBUS (PB3, rising): a USB cable.
// Stop 2 rather than Stop 1 (~4 uA more): nothing that needs Stop 1 runs then (USB is stopped, the
// ADC is off between measurements), and Stop 2 keeps SRAM, all registers and the EXTI wake-ups.
// Stop keeps the timer still, so time spent there does not count for QMK.
// The core wakes on HSI16, which is already the run clock. HSI48 (USB) stays off on battery and is
// started again when VBUS comes back.
// Without VBUS the USB driver is stopped: the connection's "auto" output picks USB while the driver
// says USB_ACTIVE, and pulling the cable does not always end that state by itself (no host left to
// send a reset or a suspend). Stopping it also switches the D+ pull-up off, which through the USB
// ESD diodes (U2) would feed the VBUS side (divider, LDO) from the battery.

#include "quantum.h"
#include <hal.h>
#include "usb_main.h"
#include "usb_util.h"
#include "ble_link.h"
#include "battery.h"
#include "power.h"

_Static_assert(STM32_SYSCLK == STM32_HSI16CLK, "SYSCLK must be HSI16 (see mcuconf.h)");

static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

static uint32_t last_busy;
static uint32_t awake_last;
static power_diag_t diag;

static bool any_key_down(void) {
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (matrix_get_row(r)) return true;
    }
    return false;
}

bool power_vbus(void) {
    return gpio_read_pin(VBUS_DET_PIN);
}

uint32_t power_wakeups(void) {
    return diag.stops;
}

const power_diag_t *power_diag(void) {
    return &diag;
}

void power_busy(void) {
    last_busy = timer_read32();
}

static void hsi48_on(void) {
    if (RCC->CRRCR & RCC_CRRCR_HSI48RDY) return;
    RCC->CRRCR |= RCC_CRRCR_HSI48ON;
    while (!(RCC->CRRCR & RCC_CRRCR_HSI48RDY)) {
    }
}

static void exti_select(pin_t pin) {
    uint32_t line  = PAL_PAD(pin);
    uint32_t port  = ((uint32_t)PAL_PORT(pin) - (uint32_t)GPIOA) / 0x400U;
    uint32_t shift = (line & 3U) * 4U;
    SYSCFG->EXTICR[line >> 2] = (SYSCFG->EXTICR[line >> 2] & ~(0xFU << shift)) | (port << shift);
}

static void stop_until_wakeup(void) {
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        gpio_set_pin_output(col_pins[c]);
        gpio_write_pin_high(col_pins[c]);
    }
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        gpio_set_pin_input_low(row_pins[r]);
    }
    wait_us(30);  // let a held key pull its row up before arming the edges

    bool key_held = false, irq_wake = false;
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        if (gpio_read_pin(row_pins[r])) key_held = true;
    }

    if (!key_held) {
        rccEnableAPB2(RCC_APB2ENR_SYSCFGEN, true);
        uint32_t rising = 1U << PAL_PAD(VBUS_DET_PIN);
        for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
            exti_select(row_pins[r]);
            rising |= 1U << PAL_PAD(row_pins[r]);
        }
        exti_select(VBUS_DET_PIN);
        exti_select(BT_IRQ_PIN);
        uint32_t falling = 1U << PAL_PAD(BT_IRQ_PIN);
        uint32_t lines   = rising | falling;

        chSysLock();
        EXTI->RTSR1 |= rising;
        EXTI->FTSR1 |= falling;
        EXTI->PR1 = lines;
        EXTI->EMR1 |= lines;
        // Checked again with the edges armed: anything that changed meanwhile is either seen here
        // or left a pending event that ends the WFE at once.
        if (!gpio_read_pin(VBUS_DET_PIN) && gpio_read_pin(BT_IRQ_PIN) && ble_link_idle()) {
            PWR->CR1 = (PWR->CR1 & ~PWR_CR1_LPMS) | PWR_CR1_LPMS_STOP2;
            SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SEVONPEND_Msk;  // a pending IRQ also wakes
            __SEV();
            __WFE();  // clears the event register
            __WFE();  // sleeps
            SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SEVONPEND_Msk);
            // By the pins: the event-only lines leave nothing in PR1 (seen 2026-11-03).
            bool key = false;
            for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
                if (gpio_read_pin(row_pins[r])) key = true;
            }
            diag.stops++;
            if (key) {
                diag.wake_key++;
            } else if (!gpio_read_pin(BT_IRQ_PIN)) {
                diag.wake_irq++;
                irq_wake = true;
            } else if (!gpio_read_pin(VBUS_DET_PIN)) {
                diag.wake_other++;
            }
        } else {
            diag.skip_busy++;
        }
        EXTI->EMR1 &= ~lines;
        EXTI->RTSR1 &= ~rising;
        EXTI->FTSR1 &= ~falling;
        EXTI->PR1 = lines;
        chSysUnlock();
        // Before anything else draws current: this may be the nRF asking for a resting sample.
        if (irq_wake) battery_rest_candidate();
        ble_link_slept();
    } else {
        diag.skip_key_held++;
    }

    // back to the matrix scan's idle state (COL2ROW: columns read with pull-ups, rows unselected)
    for (uint8_t c = 0; c < MATRIX_COLS; c++) {
        gpio_set_pin_input_high(col_pins[c]);
    }
    for (uint8_t r = 0; r < MATRIX_ROWS; r++) {
        gpio_set_pin_input_high(row_pins[r]);
    }
    last_busy = timer_read32();
}

static bool vbus_was;

void power_init(void) {
    diag.reset_flags = RCC->CSR >> 24;
    RCC->CSR |= RCC_CSR_RMVF;
    gpio_set_pin_input(VBUS_DET_PIN);  // R8 / R9 divider
    vbus_was = power_vbus();
    if (!vbus_was) usb_disconnect();
    last_busy  = timer_read32();
    awake_last = last_busy;
}

void power_task(void) {
    uint32_t now = timer_read32();
    if (!vbus_was) diag.awake_ms += now - awake_last;
    awake_last = now;

    bool vbus = power_vbus();
    if (vbus != vbus_was) {
        if (vbus) {
            hsi48_on();
            restart_usb_driver(&USB_DRIVER);
        } else {
            usb_disconnect();
            ble_link_ping();  // BLE takes over: make sure the link state is current
        }
        vbus_was = vbus;
    }

    // NO_USB_STARTUP_CHECK (Bluetooth builds) also drops QMK's suspend loop: wake the host here.
    if (USB_DRIVER.state == USB_SUSPENDED && (USB_DRIVER.status & 2U /* remote wakeup enabled */) && any_key_down()) {
        usbWakeupHost(&USB_DRIVER);
        wait_ms(20);
    }

    if (any_key_down() || !gpio_read_pin(BT_IRQ_PIN) || !ble_link_idle()) last_busy = timer_read32();

    if (!vbus && timer_elapsed32(last_busy) > LP_STOP_IDLE_MS) {
        stop_until_wakeup();
    } else {
        wait_ms(LP_SCAN_INTERVAL_MS);
    }
}
