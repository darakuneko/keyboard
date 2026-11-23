// Trident40 Wireless HY0020: what loop() was doing when it stopped (see MSG_FAULT in link_proto.h).
// The watchdog restarts the nRF when loop() has not run for NRF_WDT_TIMEOUT_MS (seen 2026-11-20,
// cause unknown: in the release build of the core a SoftDevice fault, a task stack overflow and a
// failed malloc all carry on or hang instead of resetting). A task above loop() notices earlier
// and sends the STM32 a record, which keeps it (its RAM stays powered through the nRF's reset).
#pragma once
#include <Arduino.h>

enum : uint8_t {
  STAGE_SETUP, STAGE_LINK_POLL, STAGE_FRAME, STAGE_SEND_PENDING, STAGE_TO_STM32, STAGE_STATUS,
  STAGE_CONN, STAGE_LINK_IDLE, STAGE_BAT_SAMPLE, STAGE_SYSTEM_OFF, STAGE_WAIT,
  STAGE_SLOW,   // not a stall: a link frame took FAULT_SLOW_MS or more (reported when it is done)
};
extern volatile uint8_t fault_stage;       // where loop() is (STAGE_*)
extern volatile uint8_t fault_frame_type;  // the link frame being handled (STAGE_FRAME)
extern volatile uint8_t fault_frame_arg;   // its first payload byte (the BT_CMD_* of MSG_BT_CMD)

void fault_begin(TaskHandle_t loop_task, TaskHandle_t rx_task);  // end of setup()
// Still alive: feeds the watchdog and tells the monitor. Every loop() pass, and between the steps of
// work that takes long on purpose (file system work when the BLE host changes: seconds of flash
// erases; the watchdog bit in the middle of it, 2026-11-22).
void fault_alive();
void fault_slow(uint32_t ms);   // a link frame took this long: tell the STM32 (STAGE_SLOW record)
