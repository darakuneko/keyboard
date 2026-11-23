// Copyright 2026 Daraku-Neko (@Daraku-Neko)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>

uint16_t battery_read_mv(void);          // 0 when the conversion failed
uint8_t  battery_percent(uint16_t mv);

// Resting value, for the battery level shown to the host: the median of the last
// BATTERY_REST_SAMPLES samples taken with next to no load on the cell (a CR2450 sags by 50 mV and
// more under the few mA of a key press, and by more the longer it has been typed on). Samples come
// from battery_rest_candidate() + battery_rest_commit() on battery (see MSG_BAT_SAMPLE in
// link_proto.h) and from battery_rest_push() on USB (the cell is not loaded then).
#define BATTERY_REST_SAMPLES 5
void     battery_rest_candidate(void);   // power.c: just woken from Stop by the nRF's IRQ, measure now
void     battery_rest_commit(void);      // ble_link.c: that wake-up was MSG_BAT_SAMPLE, keep the candidate
void     battery_rest_push(uint16_t mv);
uint16_t battery_rest_mv(void);          // 0: no sample yet (on battery: the first minute or so after
                                         // power-on, when start-up and connecting pull the cell down)
uint8_t  battery_rest_samples(uint16_t *out);  // newest last; returns how many
