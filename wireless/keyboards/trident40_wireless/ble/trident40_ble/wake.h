// Trident40 Wireless HY0020: loop() waits for a notification instead of delay(), so BLE callbacks and the link
// RX edge interrupt can cut a long idle wait short.
#pragma once
#include <Arduino.h>

void loop_wake_init();               // in setup() (runs in the loop task)
void loop_wait(uint32_t ms);         // end of loop(): sleep until timeout or loop_wake*()
void loop_wake();                    // from another task (BLE callbacks)
void loop_wake_from_isr();
