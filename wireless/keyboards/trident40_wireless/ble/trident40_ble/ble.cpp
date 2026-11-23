// Trident40 Wireless HY0020: BLE HID (keyboard / consumer / system / mouse) + Vial raw HID service +
// DIS + battery service. Ported from KB Bridge (darakuneko/kb_bridge, MIT).
#include <bluefruit.h>
#include "ble.h"
#include "fault.h"
#include "config.h"
#include "wake.h"
#include "stats.h"
#include <InternalFileSystem.h>
#include <utility/bonding.h>
using namespace Adafruit_LittleFS_Namespace;

// ---- main HID service ---------------------------------------------------------------------
enum { RID_KEYBOARD = 1, RID_CONSUMER, RID_SYSTEM, RID_MOUSE };

static const uint8_t main_report_map[] = {
  // keyboard: modifiers, reserved, 6 keys; LED output
  0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x85, RID_KEYBOARD,
  0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
  0x95, 0x01, 0x75, 0x08, 0x81, 0x01,
  0x05, 0x08, 0x19, 0x01, 0x29, 0x05, 0x95, 0x05, 0x75, 0x01, 0x91, 0x02,
  0x95, 0x01, 0x75, 0x03, 0x91, 0x01,
  0x05, 0x07, 0x19, 0x00, 0x2A, 0xFF, 0x00, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x95, 0x06, 0x75, 0x08,
  0x81, 0x00,
  0xC0,
  // consumer control: one 16-bit usage
  0x05, 0x0C, 0x09, 0x01, 0xA1, 0x01, 0x85, RID_CONSUMER,
  0x15, 0x00, 0x26, 0xFF, 0x03, 0x19, 0x00, 0x2A, 0xFF, 0x03, 0x95, 0x01, 0x75, 0x10, 0x81, 0x00,
  0xC0,
  // system control: one 8-bit usage (0x81 power down, 0x82 sleep, 0x83 wake up)
  0x05, 0x01, 0x09, 0x80, 0xA1, 0x01, 0x85, RID_SYSTEM,
  0x15, 0x00, 0x26, 0xB7, 0x00, 0x19, 0x00, 0x29, 0xB7, 0x95, 0x01, 0x75, 0x08, 0x81, 0x00,
  0xC0,
  // mouse: 8 buttons, X, Y, wheel, AC pan
  0x05, 0x01, 0x09, 0x02, 0xA1, 0x01, 0x85, RID_MOUSE, 0x09, 0x01, 0xA1, 0x00,
  0x05, 0x09, 0x19, 0x01, 0x29, 0x08, 0x15, 0x00, 0x25, 0x01, 0x95, 0x08, 0x75, 0x01, 0x81, 0x02,
  0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x09, 0x38, 0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x03,
  0x81, 0x06,
  0x05, 0x0C, 0x0A, 0x38, 0x02, 0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x01, 0x81, 0x06,
  0xC0, 0xC0,
};

static uint16_t input_len[] = {8, 2, 1, 5};
static uint16_t output_len[] = {1};
static BLEHidGeneric hid(4, 1, 0);
static BLEDis dis;
static BLEBas bas;

// DIS PnP ID = the keyboard's USB VID / PID (config.h).
// Value: vendor id source (2 = USB-IF), VID, PID, version, little endian.
static BLECharacteristic pnp_chr(UUID16_CHR_PNP_ID);
static const uint8_t pnp[7] = {2, (uint8_t)USB_VID, (uint8_t)(USB_VID >> 8), (uint8_t)USB_PID,
                               (uint8_t)(USB_PID >> 8), 0, 0};

// ATT MTU: the Vial report is 32 bytes. With the default 23 (20-byte payload) Bluefruit splits a
// notification into 20 + 12 bytes, which the host drops as malformed, and the host's 32-byte
// writes become long (prepared) writes.
#define ATT_MTU             64     // enough for 32-byte reports; small to keep SoftDevice RAM low
#define VIAL_MIN_MTU        (32 + 3)
// Notifications the SoftDevice may hold. Bluefruit's notify() waits up to 100 ms for a free slot;
// with the default of 1 every report waited for the previous one's connection event and loop()
// stalled long enough for the UART link to overflow.
#define HVN_QUEUE_SIZE      10

// ---- Vial raw HID service ------------------------------------------------------------------
// QMK raw HID report map (no report id): 32 bytes in, 32 bytes out.
static const uint8_t vial_report_map[] = {
  0x06, 0x60, 0xFF, 0x09, 0x61, 0xA1, 0x01,
  0x09, 0x62, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x95, 0x20, 0x75, 0x08, 0x81, 0x02,
  0x09, 0x63, 0x15, 0x00, 0x26, 0xFF, 0x00, 0x95, 0x20, 0x75, 0x08, 0x91, 0x02,
  0xC0,
};

class VialHidService : public BLEService {
 public:
  VialHidService()
      : BLEService(UUID16_SVC_HUMAN_INTERFACE_DEVICE), in_(UUID16_CHR_REPORT), out_(UUID16_CHR_REPORT),
        control_(UUID16_CHR_HID_CONTROL_POINT) {}

  err_t begin() override {
    VERIFY_STATUS(BLEService::begin());

    in_.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
    in_.setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
    in_.setFixedLen(32);
    in_.setReportRefDescriptor(0, 1);             // no report id, input
    VERIFY_STATUS(in_.begin());

    out_.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
    out_.setPermission(SECMODE_ENC_NO_MITM, SECMODE_ENC_NO_MITM);
    out_.setMaxLen(33);                           // variable length, see vial_output_report()
    out_.setReportRefDescriptor(0, 2);            // no report id, output
    out_.setWriteCallback(on_write);
    VERIFY_STATUS(out_.begin());

    BLECharacteristic map(UUID16_CHR_REPORT_MAP);
    map.setTempMemory();
    map.setProperties(CHR_PROPS_READ);
    map.setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
    map.setFixedLen(sizeof(vial_report_map));
    VERIFY_STATUS(map.begin());
    map.write(vial_report_map, sizeof(vial_report_map));

    static const uint8_t info[4] = {0x11, 0x01, 0x00, 0x02};   // HID 1.11, normally connectable
    BLECharacteristic hid_info(UUID16_CHR_HID_INFORMATION);
    hid_info.setTempMemory();
    hid_info.setProperties(CHR_PROPS_READ);
    hid_info.setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
    hid_info.setFixedLen(4);
    VERIFY_STATUS(hid_info.begin());
    hid_info.write(info, 4);

    control_.setProperties(CHR_PROPS_WRITE_WO_RESP);
    control_.setPermission(SECMODE_NO_ACCESS, SECMODE_ENC_NO_MITM);
    control_.setFixedLen(1);
    VERIFY_STATUS(control_.begin());
    return ERROR_NONE;
  }

  bool send(const uint8_t *data, uint8_t len) {
    BLEConnection *c = Bluefruit.Connection(Bluefruit.connHandle());
    if (!c || c->getMtu() < VIAL_MIN_MTU) return false;   // would go out split: never send that
    uint8_t buf[32] = {0};
    memcpy(buf, data, len > 32 ? 32 : len);
    return in_.notify(buf, 32);
  }

  static QueueHandle_t queue;
  static volatile uint16_t conn_hdl;   // connection the last Vial write came from

 private:
  // 33 bytes: BlueZ (Linux) writes report id 0 + the report to a report map without report ids.
  // Hosts that send 32 bytes are unaffected. BlueZ drops a 32-byte write from a hidraw client
  // here, so the 33-byte form is the only one that reaches us from Linux.
  static bool vial_output_report(const uint8_t *data, uint16_t len, uint8_t out[32]) {
    if (len == 0 || len > 33) return false;
    if (len == 33) {
      if (data[0] != 0) return false;
      data++;
      len--;
    }
    memset(out, 0, 32);
    memcpy(out, data, len);
    return true;
  }

  static void on_write(uint16_t conn, BLECharacteristic *, uint8_t *data, uint16_t len) {
    conn_hdl = conn;
    uint8_t buf[32];
    if (vial_output_report(data, len, buf) && queue) xQueueSend(queue, buf, 0);
    loop_wake();
  }

  BLECharacteristic in_, out_, control_;
};

QueueHandle_t VialHidService::queue;
volatile uint16_t VialHidService::conn_hdl = BLE_CONN_HANDLE_INVALID;
static VialHidService vial;
static QueueHandle_t led_queue;

// ---- callbacks -----------------------------------------------------------------------------
static void on_led_output(uint16_t, BLECharacteristic *, uint8_t *data, uint16_t len) {
  if (len && led_queue) xQueueSend(led_queue, data, 0);
  loop_wake();
}

// Connection parameters: a short interval with slave latency. The keyboard may skip up to
// `latency` empty connection events while nothing happens, but sends a key at the very next
// event, so typing latency stays one interval while the idle radio runs a few times a second.
// Hosts differ in what they accept (macOS picked 15 ms / 22 from the first set, Ubuntu took it
// as asked; some Linux kernels refuse intervals under their own 30 ms minimum, other hosts keep
// latency 0), so the sets below are tried in turn until the idle radio period is long enough.
// All of them keep Apple's rules: interval min + 15 ms <= max (or 11.25-15 for HID),
// interval x (latency + 1) <= 2 s, supervision timeout 2-6 s and > 3 x interval x (latency + 1).
// The first set is also the PPCP, which some hosts read before they choose.
static const ble_gap_conn_params_t CONN_SETS[] = {
  {9, 12, 30, 400},     // 11.25-15 ms, latency 30: idle radio every 0.35-0.47 s
  {12, 24, 15, 400},    // 15-30 ms, latency 15: 0.24-0.48 s
  {24, 40, 8, 400},     // 30-50 ms (Linux's own default range), latency 8: 0.27-0.45 s
};
#define CONN_SETS_N         (sizeof(CONN_SETS) / sizeof(CONN_SETS[0]))
#define CONN_IDLE_OK_MS     200    // idle radio period (interval x (latency + 1)) that is good enough
#define CONN_RETRY_MS       5000   // wait this long for the host to answer before the next set

static struct {
  uint16_t conn = BLE_CONN_HANDLE_INVALID;
  uint8_t next;                    // next set to ask for
  uint32_t asked_ms;               // 0: ask now
  uint32_t seen;                   // interval | latency << 16 last time it was checked
} neg;

static bool conn_request(uint16_t conn, const ble_gap_conn_params_t &p) {
  return sd_ble_gap_conn_param_update(conn, &p) == NRF_SUCCESS;   // BUSY: a request is still open
}

// Called from loop(): checks the parameters the host chose and asks for the next set while the
// idle radio period stays too short. The host may change them later (wake from sleep, another
// connection on its side): a change to a short period starts the sets again.
void ble_conn_task() {
  uint16_t hdl = Bluefruit.connHandle();
  BLEConnection *c = Bluefruit.Connection(hdl);
  if (!c || !c->connected()) {
    neg.conn = BLE_CONN_HANDLE_INVALID;
    return;
  }
  if (hdl != neg.conn) {                       // new connection: on_connect already asked for set 0
    neg.conn = hdl;
    neg.next = 1;
    neg.asked_ms = millis();
    neg.seen = 0;
  }
  uint16_t interval = c->getConnectionInterval(), latency = c->getSlaveLatency();
  uint32_t now = interval | (uint32_t)latency << 16;
  bool ok = (uint32_t)interval * 125 * (latency + 1) / 100 >= CONN_IDLE_OK_MS;
  if (now != neg.seen) {
    nrf_stats.param_changes++;
    if (neg.seen && !ok && neg.next >= CONN_SETS_N) {   // the host changed them later: try again
      neg.next = 0;
      neg.asked_ms = millis() - CONN_RETRY_MS;
    }
    neg.seen = now;
  }
  if (ok || neg.next >= CONN_SETS_N || millis() - neg.asked_ms < CONN_RETRY_MS) return;
  if (conn_request(hdl, CONN_SETS[neg.next])) neg.next++;
  neg.asked_ms = millis();
}

static void on_connect(uint16_t conn) {
  loop_wake();
  nrf_stats.connects++;
  BLEConnection *c = Bluefruit.Connection(conn);
  if (!c) return;
  conn_request(conn, CONN_SETS[0]);
  if (c->getMtu() < ATT_MTU) c->requestMtuExchange(ATT_MTU);
}

static void on_disconnect(uint16_t, uint8_t) { loop_wake(); }
static void on_secured(uint16_t) { loop_wake(); }

// ---- one host identity ------------------------------------------------------------------
// The keyboard is one BLE device (the chip's own address, DEVICE_NAME). Until 0.1.14 there were
// four host identities (BT_1..BT_4), switched by renaming bond folders: several flash page
// erases per switch, a current burst on the coin cell and, when the watchdog bit in the middle
// (2026-11-22), a risk to the file system. Removed 2026-11-22: to use another computer, disconnect
// on the computer's side; every computer that paired keeps its bond in the one bond folder.
// The file system is now written only when a host pairs (Bluefruit saves its keys) and by
// ble_erase_all().

// Advertising off and no connection: nothing can change the bonds or need the address meanwhile.
static void ble_quiet() {
  Bluefruit.Advertising.restartOnDisconnect(false);
  Bluefruit.Advertising.stop();
  for (uint16_t c = 0; c < BLE_MAX_CONNECTION; c++) {
    BLEConnection *conn = Bluefruit.Connection(c);
    if (conn && conn->connected()) conn->disconnect();
  }
  uint32_t t = millis();
  while (Bluefruit.connected() && millis() - t < 1000) delay(10);
  delay(200);   // Bluefruit saves the CCCDs of a closed connection from its callback task
  Bluefruit.Advertising.stop();
  fault_alive();
}

// ---- public --------------------------------------------------------------------------------
void ble_begin() {
  VialHidService::queue = xQueueCreate(8, 32);
  led_queue = xQueueCreate(4, 1);

  Bluefruit.configPrphConn(ATT_MTU, BLE_GAP_EVENT_LENGTH_DEFAULT, HVN_QUEUE_SIZE,
                           BLE_GATTC_WRITE_CMD_TX_QUEUE_SIZE_DEFAULT);
  Bluefruit.begin(1, 0);
  if (NRF_DCDC_ENABLE) sd_power_dcdc_mode_set(NRF_POWER_DCDC_ENABLE);   // see config.h
  Bluefruit.setTxPower(0);                       // a keyboard on the desk: +4 dBm only costs current
  Bluefruit.Periph.setConnectCallback(on_connect);
  Bluefruit.Periph.setDisconnectCallback(on_disconnect);
  Bluefruit.Security.setSecuredCallback(on_secured);
  Bluefruit.Periph.setConnInterval(CONN_SETS[0].min_conn_interval, CONN_SETS[0].max_conn_interval);
  Bluefruit.Periph.setConnSlaveLatency(CONN_SETS[0].slave_latency);
  Bluefruit.Periph.setConnSupervisionTimeout(CONN_SETS[0].conn_sup_timeout);

  // "vial:f64c2b3c:" + 6 hex digits = 20 bytes (the DIS string limit): the Vial GUI looks for the
  // magic, so it must come first (RMK does the same).
  static char serial[21];
  snprintf(serial, sizeof(serial), "vial:f64c2b3c:%06lX", (unsigned long)(NRF_FICR->DEVICEID[0] & 0xFFFFFF));
  dis.setManufacturer(MANUFACTURER_NAME);
  dis.setModel(MODEL_NAME);
  dis.setSerialNum(serial);
  dis.setFirmwareRev(FW_VERSION_STR);
  dis.begin();
  pnp_chr.setProperties(CHR_PROPS_READ);                  // added to the DIS (the last service begun)
  pnp_chr.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  pnp_chr.setFixedLen(7);
  pnp_chr.begin();
  pnp_chr.write(pnp, 7);

  hid.enableKeyboard(true);
  hid.enableMouse(true);
  hid.setReportLen(input_len, output_len, nullptr);
  hid.setReportMap(main_report_map, sizeof(main_report_map));
  hid.begin();
  hid.setOutputReportCallback(RID_KEYBOARD, on_led_output);

  vial.begin();

  bas.begin();
  bas.write(100);

  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(hid);
  InternalFS.begin();
  Bluefruit.setName(DEVICE_NAME);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);        // 20 ms fast, 152.5 ms slow
  Bluefruit.Advertising.setFastTimeout(ADV_FAST_TIMEOUT_S);
  Bluefruit.Advertising.start(0);                    // forever
}

void ble_battery(uint8_t percent) { bas.notify(percent); }

bool ble_connected() { return Bluefruit.connected() > 0; }

void ble_conn_params(uint16_t &interval, uint16_t &latency, uint16_t &timeout) {
  BLEConnection *c = Bluefruit.Connection(Bluefruit.connHandle());
  bool on = c && c->connected();
  interval = on ? c->getConnectionInterval() : 0;
  latency = on ? c->getSlaveLatency() : 0;
  timeout = on ? c->getSupervisionTimeout() : 0;
}

// Connected to a host that has paired (reports only go out on an encrypted link).
bool ble_secured() {
  BLEConnection *c = Bluefruit.Connection(Bluefruit.connHandle());
  return c && c->connected() && c->secured();
}
bool ble_advertising() { return Bluefruit.Advertising.isRunning(); }

void ble_start_advertising() {
  if (!Bluefruit.Advertising.isRunning()) Bluefruit.Advertising.start(0);
}

// Every pairing gone: the file system formatted, then a restart (Bluefruit
// sets its bond folders up again at start). The way back when the file system is corrupt (a reset
// in the middle of a write): then no bond is ever saved again, and forgetting a host cannot help.
void ble_erase_all() {
  ble_quiet();
  InternalFS.format();
  fault_alive();
  delay(50);
  NVIC_SystemReset();
}

bool ble_keyboard(const uint8_t report[8]) {
  if (!ble_connected()) return false;
  uint8_t r[8];
  memcpy(r, report, 8);
  if (hid.isBootMode()) return hid.bootKeyboardReport(r, 8);
  return hid.inputReport(RID_KEYBOARD, r, 8);
}

bool ble_consumer(uint16_t usage) {
  if (!ble_connected()) return false;
  if (hid.isBootMode()) return true;               // no consumer page in boot protocol: drop
  uint8_t r[2] = {(uint8_t)usage, (uint8_t)(usage >> 8)};
  return hid.inputReport(RID_CONSUMER, r, 2);
}

bool ble_system(uint8_t usage) {
  if (!ble_connected()) return false;
  if (hid.isBootMode()) return true;
  return hid.inputReport(RID_SYSTEM, &usage, 1);
}

bool ble_mouse(const uint8_t report[5]) {
  if (!ble_connected()) return false;
  uint8_t r[5];
  memcpy(r, report, 5);
  if (hid.isBootMode()) return hid.bootMouseReport(r, 5);
  return hid.inputReport(RID_MOUSE, r, 5);
}

bool ble_vial_input(const uint8_t *data, uint8_t len) {
  return ble_connected() && vial.send(data, len);
}

bool ble_take_leds(uint8_t &leds) { return led_queue && xQueueReceive(led_queue, &leds, 0) == pdTRUE; }

// While Vial is talking, listen at every connection event. With slave latency the host's writes
// arrive bunched (up to (latency + 1) x interval late), and Vial's unlock poll restarts its count
// when two polls reach the keyboard less than 100 ms apart; everything else is just slow.
static void set_latency_off(uint16_t conn, bool off) {
  ble_opt_t opt = {};
  opt.gap_opt.slave_latency_disable.conn_handle = conn;
  opt.gap_opt.slave_latency_disable.disable = off;
  sd_ble_opt_set(BLE_GAP_OPT_SLAVE_LATENCY_DISABLE, &opt);
  if (off && !nrf_stats.latency_off) nrf_stats.latency_off_since = millis();
  if (!off && nrf_stats.latency_off) nrf_stats.latency_off_ms += millis() - nrf_stats.latency_off_since;
  nrf_stats.latency_off = off;
}

bool ble_take_vial_output(uint8_t buf[32]) {
  static uint16_t off_conn = BLE_CONN_HANDLE_INVALID;
  static uint32_t last_ms;
  bool got = VialHidService::queue && xQueueReceive(VialHidService::queue, buf, 0) == pdTRUE;
  if (got) {
    last_ms = millis();
    uint16_t conn = VialHidService::conn_hdl;
    if (conn != off_conn) {
      if (off_conn != BLE_CONN_HANDLE_INVALID) set_latency_off(off_conn, false);
      set_latency_off(conn, true);
      off_conn = conn;
    }
  } else if (off_conn != BLE_CONN_HANDLE_INVALID && millis() - last_ms > VIAL_LATENCY_OFF_MS) {
    set_latency_off(off_conn, false);   // fails harmlessly if that connection is gone
    off_conn = BLE_CONN_HANDLE_INVALID;
  }
  return got;
}
