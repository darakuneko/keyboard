// Trident40 Wireless HY0020: power counters since the last start, sent as MSG_STATS (link_proto.h)
// when the STM32 asks (tools/diag.py). They say where the nRF spends its time, to follow a battery
// drain that grows over days (2026-11-18: the load drop came back partly after an nRF reset).
#pragma once
#include <Arduino.h>

struct NrfStats {
  uint32_t loop_wakes;      // loop() passes
  uint32_t busy_ms;         // time loop() ran every LOOP_BUSY_MS (reports waiting for BLE)
  uint32_t uart_on_ms;      // time the link receiver was on (~0.9 mA)
  uint32_t uart_starts;
  uint32_t latency_off_ms;  // time slave latency was off (Vial talking: radio at every interval)
  uint16_t connects;
  uint16_t param_changes;   // connection parameters the host set
  uint32_t uart_on_since;   // millis() when the receiver went on / latency went off
  uint32_t latency_off_since;
  bool     latency_off;
};
extern NrfStats nrf_stats;
