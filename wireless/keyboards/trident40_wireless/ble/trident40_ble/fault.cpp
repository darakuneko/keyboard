// Trident40 Wireless HY0020: loop() stall record (see fault.h).
#include "fault.h"
#include "config.h"
#include "link.h"
#include "stats.h"

volatile uint8_t fault_stage, fault_frame_type, fault_frame_arg;
static volatile uint32_t alive_count;
static TaskHandle_t loop_task, rx_task;

extern unsigned long __text_start__ __attribute__((weak));
extern unsigned long __etext;

static bool is_code(uint32_t v) {
  return (v & 1) && v >= 0x26000 && v < (uint32_t)&__etext;   // Thumb address inside the application
}

static uint8_t free_words(TaskHandle_t t) {
  UBaseType_t n = uxTaskGetStackHighWaterMark(t);
  return n > 255 ? 255 : (uint8_t)n;
}

void fault_alive() {
  NRF_WDT->RR[0] = WDT_RR_RR_Reload;
  alive_count++;
}

void fault_slow(uint32_t ms) {
  uint8_t r[30] = {0};
  r[0] = STAGE_SLOW;
  r[1] = fault_frame_type;
  r[3] = free_words(loop_task);
  r[4] = free_words(rx_task);
  r[5] = fault_frame_arg;
  uint32_t v[2] = {millis() / 1000, ms};           // the duration where a stall has its pc
  memcpy(r + 6, v, 8);
  link_send(MSG_FAULT, r, sizeof(r));
}

static void send_record() {
  uint8_t r[30] = {0};
  r[0] = fault_stage;
  r[1] = fault_frame_type;
  r[2] = (uint8_t)eTaskGetState(loop_task);
  r[3] = free_words(loop_task);
  r[4] = free_words(rx_task);
  r[5] = fault_frame_arg;
  uint32_t v[6] = {millis() / 1000, 0, 0, 0, 0, 0};
  // The loop task's saved context (FreeRTOS ARM_CM4F port): r4-r11 and EXC_RETURN, s16-s31 when
  // the task used the FPU, then the frame the core pushed: r0-r3, r12, lr, pc, xpsr.
  uint32_t *sp = *(uint32_t **)loop_task;          // pxTopOfStack is the TCB's first member
  uint32_t *frame = sp + 9 + ((sp[8] & 0x10) ? 0 : 16);
  v[1] = frame[6];
  v[2] = frame[5];
  // Return addresses further up the stack: the callers of whatever is waiting (no unwinder here).
  uint8_t n = 3;
  for (uint32_t *p = frame + 8 + ((sp[8] & 0x10) ? 0 : 18); n < 6 && p < frame + 200; p++) {
    if (is_code(*p) && *p != v[n - 1]) v[n++] = *p;
  }
  memcpy(r + 6, v, 24);
  link_send(MSG_FAULT, r, sizeof(r));
}

static void monitor(void *) {
  uint32_t seen = 0, since = millis();
  bool sent = false;
  for (;;) {
    vTaskDelay(pdMS_TO_TICKS(FAULT_CHECK_MS));
    if (alive_count != seen) {
      seen = alive_count;
      since = millis();
      sent = false;
    } else if (!sent && millis() - since >= FAULT_STALL_MS) {
      sent = true;                                 // once per stall; the watchdog comes next
      send_record();
    }
  }
}

void fault_begin(TaskHandle_t loop, TaskHandle_t rx) {
  loop_task = loop;
  rx_task = rx;
  xTaskCreate(monitor, "fault", 256, nullptr, TASK_PRIO_HIGH, nullptr);
}
