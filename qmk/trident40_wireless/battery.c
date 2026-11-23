// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later
//
// Battery voltage: VBAT (PB1, ADC1_IN16) against VREFINT (ADC1_IN0). VDDA is the 3.0 V boost
// output on battery and 3.3 V on USB, so it is worked out from VREFINT, not assumed. The ADC is
// started for one conversion and stopped again (deep power-down in between).

#include "quantum.h"
#include <hal.h>
#include "battery.h"

#define VREFINT_CAL (*(const uint16_t *)0x1FFF75AAU)  // ADC raw of VREFINT at VDDA = 3.0 V, 30 C
#define VREFINT_CAL_MV 3000

static const ADCConversionGroup battery_group = {
    .circular     = false,
    .num_channels = 2,
    .cfgr         = 0,  // 12 bit, single conversion
    .smpr         = {ADC_SMPR1_SMP_AN0(ADC_SMPR_SMP_247P5), ADC_SMPR2_SMP_AN16(ADC_SMPR_SMP_247P5)},
    .sqr          = {ADC_SQR1_SQ1_N(ADC_CHANNEL_IN0) | ADC_SQR1_SQ2_N(ADC_CHANNEL_IN16), 0, 0, 0},
};

uint16_t battery_read_mv(void) {
    static adcsample_t buf[2];
    palSetLineMode(VBAT_ADC_PIN, PAL_MODE_INPUT_ANALOG);
    adcStart(&ADCD1, NULL);
    adcSTM32EnableVREF(&ADCD1);
    wait_us(20);  // VREFINT start-up
    msg_t ok = adcConvert(&ADCD1, &battery_group, buf, 1);
    adcSTM32DisableVREF(&ADCD1);
    adcStop(&ADCD1);
    if (ok != MSG_OK || buf[0] == 0) return 0;
    return (uint16_t)((uint32_t)VREFINT_CAL_MV * VREFINT_CAL / buf[0] * buf[1] / 4095U);
}

uint8_t battery_percent(uint16_t mv) {
    if (mv >= BATTERY_FULL_MV) return 100;
    if (mv <= BATTERY_EMPTY_MV) return 0;
    return (uint8_t)((uint32_t)(mv - BATTERY_EMPTY_MV) * 100U / (BATTERY_FULL_MV - BATTERY_EMPTY_MV));
}

// ---- resting value (battery.h) -------------------------------------------------------------
static uint16_t rest[BATTERY_REST_SAMPLES];
static uint8_t  rest_count;
static uint16_t candidate;

void battery_rest_candidate(void) {
    candidate = battery_read_mv();
}

void battery_rest_commit(void) {
    if (candidate) battery_rest_push(candidate);
    candidate = 0;
}

void battery_rest_push(uint16_t mv) {
    if (!mv) return;
    if (rest_count == BATTERY_REST_SAMPLES) {
        memmove(rest, rest + 1, sizeof(rest) - sizeof(rest[0]));
        rest_count--;
    }
    rest[rest_count++] = mv;
}

uint8_t battery_rest_samples(uint16_t *out) {
    memcpy(out, rest, sizeof(rest[0]) * rest_count);
    return rest_count;
}

uint16_t battery_rest_mv(void) {
    if (!rest_count) return 0;
    uint16_t s[BATTERY_REST_SAMPLES];
    memcpy(s, rest, sizeof(s));
    for (uint8_t i = 1; i < rest_count; i++) {  // insertion sort
        uint16_t v = s[i];
        int8_t   j = i - 1;
        for (; j >= 0 && s[j] > v; j--) s[j + 1] = s[j];
        s[j + 1] = v;
    }
    return rest_count & 1 ? s[rest_count / 2] : (uint16_t)((s[rest_count / 2 - 1] + s[rest_count / 2]) / 2);
}
