// HY0020 (nRF52832) update through the keyboard's STM32: the STM32 tunnels the nRF bootloader's
// serial DFU through Vial's raw HID interface (hy0020_update.c in the QMK keyboard). WebHID port of
// keyboards/trident40_wireless/tools/hy0020_update.py.
import { dfuPackets, ACTIVATE_WAIT_S } from './nrfdfu.js';

const RAW_USAGE_PAGE = 0xff60;
const RAW_USAGE = 0x61;
const UPD_WRITE = 0xb0;
const UPD_READ = 0xb1;
const UPD_CTRL = 0xb2;
const UPD_ERASE = 0xb5;
export const CTRL_STATE = 0;
export const CTRL_RESET = 1;
export const CTRL_EXIT = 2;
export const CTRL_STM32_BOOT = 4;

const ACK_TIMEOUT_MS = 1000;
const SEND_ATTEMPTS = 4;        // the bootloader drops a duplicate sequence number and only acknowledges it
const ENTER_ATTEMPTS = 5;
// The bootloader waits for the first packet only about 3 s after the keyboard sent the HY0020
// there (then it starts the application again): START goes out this long after update mode is seen.
const BOOTLOADER_START_MS = 1000;
const RESET_TO_START_MS = 100;
const POLL_MS = 300;

export const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

export function hidFilters(kb) {
  return [{ vendorId: kb.usb.vid, productId: kb.usb.pid, usagePage: RAW_USAGE_PAGE, usage: RAW_USAGE }];
}

// One opened keyboard (its raw HID collection).
export class Tunnel {
  constructor(device) {
    this.dev = device;
    this.rx = [];            // bytes from the nRF
    this.answers = [];       // other reports
    this.waiter = null;
    this.gone = false;
    this.onReport = (e) => {
      const d = new Uint8Array(e.data.buffer, e.data.byteOffset, e.data.byteLength);
      if (d[0] === UPD_READ) {
        for (let i = 0; i < d[1] && 2 + i < d.length; i++) this.rx.push(d[2 + i]);
      } else {
        this.answers.push(Uint8Array.from(d));
      }
      this.wake();
    };
    this.onDisconnect = (e) => {
      if (e.device === this.dev) {
        this.gone = true;
        this.wake();
      }
    };
  }

  wake() {
    if (this.waiter) this.waiter();
  }

  async open() {
    if (!this.dev.opened) await this.dev.open();
    this.dev.addEventListener('inputreport', this.onReport);
    navigator.hid.addEventListener('disconnect', this.onDisconnect);
  }

  async close() {
    this.dev.removeEventListener('inputreport', this.onReport);
    navigator.hid.removeEventListener('disconnect', this.onDisconnect);
    try { await this.dev.close(); } catch (e) { /* already gone */ }
  }

  async wait(ms) {
    await new Promise((r) => {
      this.waiter = r;
      setTimeout(r, ms);
    });
    this.waiter = null;
  }

  async send(report) {
    if (this.gone) throw new Error('GONE');
    const out = new Uint8Array(32);
    out.set(report);
    try {
      await this.dev.sendReport(0, out);
    } catch (e) {
      throw new Error('GONE');
    }
  }

  // The answer to `cmd` (reports echo their command byte), or null after the timeout.
  async answer(cmd, timeoutMs = 1000) {
    const deadline = Date.now() + timeoutMs;
    for (;;) {
      const i = this.answers.findIndex((r) => r[0] === cmd);
      if (i >= 0) {
        const r = this.answers[i];
        this.answers.splice(0, i + 1);
        return r;
      }
      if (this.gone) throw new Error('GONE');
      const left = deadline - Date.now();
      if (left <= 0) return null;
      await this.wait(left);
    }
  }

  // Returns true while the keyboard is in update mode.
  async ctrl(op, timeoutMs = 1000) {
    this.answers.length = 0;
    await this.send([UPD_CTRL, op]);
    const r = await this.answer(UPD_CTRL, timeoutMs);
    return !!(r && r[2]);
  }

  async write(data) {
    for (let i = 0; i < data.length; i += 30) {
      const chunk = data.subarray(i, i + 30);
      await this.send([UPD_WRITE, chunk.length, ...chunk]);
      const r = await this.answer(UPD_WRITE);
      if (!r || r[1] !== chunk.length) throw new Error('LEFT_UPDATE_MODE');
    }
  }

  // One SLIP frame from the nRF, or null after the timeout.
  async getAck() {
    const deadline = Date.now() + ACK_TIMEOUT_MS;
    for (;;) {
      const first = this.rx.indexOf(0xc0);
      const second = first < 0 ? -1 : this.rx.indexOf(0xc0, first + 1);
      if (second > first + 1) return this.rx.splice(0, second + 1).slice(first + 1, second);
      if (second === first + 1 && first >= 0) {   // two delimiters back to back: keep the second as a start
        this.rx.splice(0, second);
        continue;
      }
      if (this.gone) throw new Error('GONE');
      const left = deadline - Date.now();
      if (left <= 0) return null;
      await this.wait(Math.min(left, 50));
    }
  }

  // Erase every pairing (the HY0020 application formats its file system and restarts). Retried
  // while the application starts up. Not in update mode, and only with the USB cable connected.
  async erasePairings(waitMs = 15000) {
    const deadline = Date.now() + waitMs;
    while (Date.now() < deadline) {
      this.answers.length = 0;
      await this.send([UPD_ERASE]);
      const r = await this.answer(UPD_ERASE);
      if (r && r[1]) return true;
      await sleep(500);
    }
    return false;
  }
}

// The chooser. Returns an opened Tunnel, or null when the user closed it.
export async function requestKeyboard(kb) {
  let devices;
  try {
    devices = await navigator.hid.requestDevice({ filters: hidFilters(kb) });
  } catch (e) {
    return null;
  }
  const dev = devices.find((d) => d.collections.some((c) => c.usagePage === RAW_USAGE_PAGE)) || devices[0];
  if (!dev) return null;
  const t = new Tunnel(dev);
  try {
    await t.open();
  } catch (e) {
    throw new Error('OPEN');
  }
  return t;
}

// Waits until the update key was pressed. A keyboard that is already in update mode is taken out of
// it first: its bootloader may have given up waiting (see BOOTLOADER_START_MS), a fresh key press
// starts from a known point. onState('pressKey' | 'pressKeyAgain').
export async function waitForUpdateMode(t, onState, { fresh = true, timeoutMs = 180000 } = {}) {
  if (await t.ctrl(CTRL_STATE)) {
    if (!fresh) return;
    await t.ctrl(CTRL_EXIT);
    await sleep(2000);
    onState('pressKeyAgain');
  } else {
    onState('pressKey');
  }
  const deadline = Date.now() + timeoutMs;
  while (!(await t.ctrl(CTRL_STATE))) {
    if (Date.now() > deadline) throw new Error('NO_KEY');
    await sleep(POLL_MS);
  }
}

async function tryStart(t, pkt) {
  t.rx.length = 0;                       // whatever the application printed
  await t.write(pkt);
  return (await t.getAck()) !== null;
}

// Writes one application package. The keyboard must just have entered update mode
// (waitForUpdateMode). onProgress(doneBytes, totalBytes, phase).
export async function flashHy0020(t, bin, dat, onProgress = () => {}) {
  const steps = dfuPackets(bin, dat);
  onProgress(0, bin.length, 'erase');
  await sleep(BOOTLOADER_START_MS);      // the application restarts in the bootloader
  let ok = await tryStart(t, steps[0].pkt);
  if (!ok) {
    // First write: the HY0020 still runs the hy0020_checker firmware, which does not know the
    // keyboard's link. Its serial command "d" (115200 baud, what the tunnel uses) enters the bootloader.
    await t.write(Uint8Array.from([0x64]));
    await sleep(BOOTLOADER_START_MS);
    ok = await tryStart(t, steps[0].pkt);
  }
  for (let i = 0; i < ENTER_ATTEMPTS && !ok; i++) {
    // a reset pulse: after it the bootloader listens for a moment (needs P0.21 as the reset pin)
    await t.ctrl(CTRL_RESET);
    await sleep(RESET_TO_START_MS);
    ok = await tryStart(t, steps[0].pkt);
  }
  if (!ok) {
    await t.ctrl(CTRL_EXIT);
    throw new Error('NO_BOOTLOADER');
  }
  await sleep(steps[0].wait * 1000);
  for (const s of steps.slice(1)) {
    let ack = null;
    for (let a = 0; a < SEND_ATTEMPTS && ack === null; a++) {
      t.rx.length = 0;                   // a late ACK of an earlier try must not count for this one
      await t.write(s.pkt);
      ack = await t.getAck();
    }
    if (ack === null) {
      await t.ctrl(CTRL_EXIT);
      throw new Error('NO_ACK');
    }
    if (s.wait) await sleep(s.wait * 1000);
    if (s.kind === 'data') onProgress(s.done, bin.length, 'write');
  }
  onProgress(bin.length, bin.length, 'activate');
  await sleep(ACTIVATE_WAIT_S * 1000 + 500);
  await t.ctrl(CTRL_EXIT);               // the HY0020 starts its application, the BLE link comes back
}
