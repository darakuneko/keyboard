// Trident40 Wireless HY0020: loop() wake-up (see wake.h).
#include "wake.h"

static TaskHandle_t loop_task;

void loop_wake_init() { loop_task = xTaskGetCurrentTaskHandle(); }

void loop_wait(uint32_t ms) { ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(ms)); }

void loop_wake() {
  if (loop_task) xTaskNotifyGive(loop_task);
}

void loop_wake_from_isr() {
  if (!loop_task) return;
  BaseType_t woken = pdFALSE;
  vTaskNotifyGiveFromISR(loop_task, &woken);
  portYIELD_FROM_ISR(woken);
}
