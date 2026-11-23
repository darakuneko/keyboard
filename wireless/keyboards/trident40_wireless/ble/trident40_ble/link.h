// Trident40 Wireless HY0020: framed UART link to the STM32 (see link_proto.h).
#pragma once
#include <Arduino.h>
#include "link_proto.h"

struct LinkFrame {
  uint8_t type;
  uint8_t len;
  uint8_t data[LINK_MAX_PAYLOAD];
};

void link_begin();
TaskHandle_t link_rx_task();
bool link_uart_on();
void link_idle_task();          // every loop(): switches the receiver off after LINK_UART_IDLE_MS
void link_system_off_prepare(); // UART off, RX set to wake the chip from System OFF
bool link_poll(LinkFrame &f);   // true when a complete frame arrived
void link_send(uint8_t type, const uint8_t *payload, uint8_t len);
uint16_t link_rx_frames();
uint16_t link_rx_dropped();     // bad CRC, oversize length or a gap inside a frame
