// Trident40 Wireless HY0020 (nRF52832): build-time settings.
#pragma once

#define FW_VERSION_MAJOR    0
#define FW_VERSION_MINOR    1
#define FW_VERSION_PATCH    15
#define FW_VERSION_STR      "0.1.15"

#define DEVICE_NAME         "Trident40 Wireless"
#define MANUFACTURER_NAME   "Daraku-Neko"
#define MODEL_NAME          "Trident40 Wireless"
// USB ids of the keyboard, also the BLE PnP ID (the Vial GUI matches on them).
#define USB_VID             0x7A79
#define USB_PID             0x1577

// ---- pins (hy0020_sip: 1 DFU P0.20, 2 RST P0.21, 3 IRQ P0.07, 4 RX P0.08, 5 TX P0.06) ----------
#define PIN_LINK_TX         6     // -> STM32 PB7 (USART1 RX)  (UARTE0, link.cpp)
#define PIN_LINK_RX         8     // <- STM32 PB6 (USART1 TX)
#define PIN_IRQ             7     // -> STM32 PA1: low = a frame follows (wakes the STM32 from Stop)

// ---- BLE -----------------------------------------------------------------------------------
#define ADV_FAST_TIMEOUT_S  30
// Slave latency stays off this long after the last Vial message (the GUI polls while open).
#define VIAL_LATENCY_OFF_MS 10000

// ---- power ---------------------------------------------------------------------------------
// nRF52832 internal DC/DC: hy0020_sip ties DCC (M1) to DEC4 (N1), as the HY0020 datasheet asks for.
#define NRF_DCDC_ENABLE     true
// System OFF (~0.5 uA) after this long without a connection. The STM32's wake byte (RX low) starts
// the nRF again (a reset): any key on the keyboard does that.
#ifndef SYSTEM_OFF_IDLE_MS
#define SYSTEM_OFF_IDLE_MS  300000    // build.sh: EXTRA_FLAGS=-DSYSTEM_OFF_IDLE_MS=30000 to test
#endif
// loop() period while reports wait for BLE; otherwise loop() sleeps until link bytes, a BLE event
// or LOOP_UART_MS (the receiver is on: link_idle_task() switches it off on time) / LOOP_SLEEP_MS.
#define LOOP_BUSY_MS        1
#define LOOP_UART_MS        20        // well under LINK_UART_IDLE_MS, or the receiver stays on that much longer
#define LOOP_SLEEP_MS       1000

// ---- stability -----------------------------------------------------------------------------
#define NRF_WDT_TIMEOUT_MS  4000      // fed every loop(); not running during setup()
// loop() not run for this long: a record goes to the STM32 before the watchdog bites (fault.h).
#define FAULT_STALL_MS       2000
#define FAULT_CHECK_MS       250
#define FAULT_SLOW_MS        1000      // a link frame that took this long is reported too (fault.h)
