// Trident40 Wireless HY0020: BLE side. One HID service for input (keyboard, consumer, system, mouse)
// and a second HID service for Vial raw HID (usage page 0xFF60), like RMK: Android only uses the
// first. Device Information Service serial = "vial:f64c2b3c:XXXXXX" so the Vial GUI finds the device.
#pragma once
#include <Arduino.h>

void ble_begin();
bool ble_connected();
bool ble_secured();                           // connected and encrypted: reports can go out
bool ble_advertising();
void ble_start_advertising();
void ble_erase_all();                         // format the file system (every bond), restart
void ble_battery(uint8_t percent);
void ble_conn_params(uint16_t &interval, uint16_t &latency, uint16_t &timeout);   // all 0 when not connected
void ble_conn_task();                         // loop(): ask the host for other connection parameters while the idle radio runs too often

// Report senders return false when the report did not go out (not connected, notifications not
// enabled yet, or the SoftDevice queue is full); the caller keeps it and tries again.
bool ble_keyboard(const uint8_t report[8]);
bool ble_consumer(uint16_t usage);
bool ble_system(uint8_t usage);
bool ble_mouse(const uint8_t report[5]);
bool ble_vial_input(const uint8_t *data, uint8_t len);

// Filled from BLE callbacks (other task), drained by loop():
bool ble_take_leds(uint8_t &leds);            // keyboard LED output report from the host
bool ble_take_vial_output(uint8_t buf[32]);   // Vial raw HID output report from the host
