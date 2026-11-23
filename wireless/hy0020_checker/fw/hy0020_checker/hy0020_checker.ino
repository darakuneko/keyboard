// HY0020 checker: self-test firmware for a reflowed hy0020_sip on the hy0020_checker board.
// Board: Adafruit Feather nRF52832 (adafruit:nrf52:feather52832). Build with ../build.sh.
//
// Checker wiring (SIP pins): P0.06 TX / P0.08 RX -> CH340C (115200), P0.07 -> D1 (LED),
// P0.20 -> SW2 (DFU/PAIR, low = pressed), P0.21 -> SW1 RESET.
//
// What it checks:
//   UART   the report on the CH340C port (and the serial commands) works both ways
//   CPU    device id, chip temperature, VDD
//   BLE    advertises as "HY0020 XXXX"; once a phone or PC connects, the RSSI is printed every second
//   GPIO   D1 blinks the state; pressing SW2 lights D1 and prints "SW2 pressed"
// D1: double blink every second = advertising, steady on = connected (different from the factory
// sample's plain 1 Hz blink, so a board still running the sample is easy to tell apart).
//
// Serial commands: ? report again, d serial DFU (write new firmware), r reset.
#include <bluefruit.h>

#define FW_VERSION    "1.0"
#define PIN_D1        7
#define PIN_SW2       20
#define REPORT_MS     1000

static char name[16];
static bool sw2_down;

static void report() {
  analogReference(AR_INTERNAL);                 // 0.6 V x 6 = 3.6 V full scale
  analogReadResolution(12);
  uint32_t vdd_mv = analogReadVDD() * 3600UL / 4095UL;
  uint8_t mac[6];
  Bluefruit.getAddr(mac);
  Serial.println();
  Serial.println("HY0020 checker " FW_VERSION);
  Serial.printf("device id   %08lX%08lX\n", (unsigned long)NRF_FICR->DEVICEID[1], (unsigned long)NRF_FICR->DEVICEID[0]);
  Serial.printf("BLE address %02X:%02X:%02X:%02X:%02X:%02X\n", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.printf("BLE name    %s\n", name);
  Serial.printf("VDD         %lu mV\n", (unsigned long)vdd_mv);
  Serial.printf("temperature %.1f C\n", readCPUTemperature());
  Serial.printf("SW2         %s\n", digitalRead(PIN_SW2) == LOW ? "pressed" : "released");
  Serial.println("commands: ? report  d serial DFU  r reset");
}

static void on_connect(uint16_t conn) {
  BLEConnection *c = Bluefruit.Connection(conn);
  if (c) c->monitorRssi();
  Serial.println("BLE connected");
}

static void on_disconnect(uint16_t, uint8_t reason) {
  Serial.printf("BLE disconnected (reason 0x%02X)\n", reason);
}

static void led_task(uint32_t now) {
  bool on;
  if (sw2_down || Bluefruit.connected()) {
    on = true;
  } else {
    uint32_t p = now % 1000;                    // double blink: 0-80 ms, 200-280 ms
    on = p < 80 || (p >= 200 && p < 280);
  }
  digitalWrite(PIN_D1, on ? HIGH : LOW);
}

void setup() {
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_SW2, INPUT_PULLUP);
  Serial.begin(115200);

  Bluefruit.autoConnLed(false);                 // the Feather's LED pin is not wired on the SIP
  Bluefruit.begin();
  Bluefruit.setTxPower(4);
  snprintf(name, sizeof(name), "HY0020 %04lX", (unsigned long)(NRF_FICR->DEVICEID[0] & 0xFFFF));
  Bluefruit.setName(name);
  Bluefruit.Periph.setConnectCallback(on_connect);
  Bluefruit.Periph.setDisconnectCallback(on_disconnect);
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.start(0);

  report();
}

void loop() {
  static uint32_t last_ms;
  uint32_t now = millis();

  bool down = digitalRead(PIN_SW2) == LOW;
  if (down != sw2_down) {
    sw2_down = down;
    Serial.println(down ? "SW2 pressed" : "SW2 released");
  }
  led_task(now);

  if (now - last_ms >= REPORT_MS) {
    last_ms = now;
    BLEConnection *c = Bluefruit.Connection(0);
    if (c && c->connected()) Serial.printf("RSSI %d dBm\n", c->getRssi());
  }

  while (Serial.available()) {
    switch (Serial.read()) {
      case '?': report(); break;
      case 'd': Serial.println("entering serial DFU"); Serial.flush(); delay(50); enterSerialDfu(); break;
      case 'r': Serial.println("reset"); Serial.flush(); delay(50); NVIC_SystemReset(); break;
      default: break;
    }
  }
  delay(10);
}
