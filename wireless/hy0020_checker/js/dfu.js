// Adafruit nRF52 legacy serial DFU (what `adafruit-nrfutil dfu serial --singlebank` does), over Web Serial.
// Packets: HCI header + payload + CRC16, SLIP framed. Timings follow nordicsemi/dfu/dfu_transport_serial.py.

const DFU_INIT_PACKET = 1;
const DFU_START_PACKET = 3;
const DFU_DATA_PACKET = 4;
const DFU_STOP_DATA_PACKET = 5;
const DFU_UPDATE_MODE_APP = 4;

const HCI_PACKET_TYPE = 14;
const DATA_PACKET_MAX = 512;
const FLASH_PAGE_SIZE = 4096;
const FLASH_PAGE_ERASE_S = 0.0897;
const FLASH_PAGE_WRITE_S = (FLASH_PAGE_SIZE / 4) * 0.0001;
const ACK_TIMEOUT_MS = 1000;
// A packet without an ACK is sent again with the same sequence number: the bootloader's HCI layer
// drops a duplicate and only acknowledges it, so a lost byte costs a retry instead of the update.
const SEND_ATTEMPTS = 4;

export function crc16(bytes, crc = 0xffff) {
  for (const b of bytes) {
    crc = ((crc >> 8) & 0xff) | ((crc << 8) & 0xff00);
    crc ^= b;
    crc ^= (crc & 0xff) >> 4;
    crc ^= (crc << 12) & 0xffff;
    crc ^= ((crc & 0xff) << 5) & 0xffff;
  }
  return crc & 0xffff;
}

const u32 = (v) => [v & 0xff, (v >>> 8) & 0xff, (v >>> 16) & 0xff, (v >>> 24) & 0xff];

// Builds SLIP-framed HCI packets with the running sequence number (the first packet is 1).
export class PacketBuilder {
  constructor() { this.seq = 0; }

  build(payload) {
    this.seq = (this.seq + 1) % 8;
    const len = payload.length;
    const h = [
      this.seq | (((this.seq + 1) % 8) << 3) | (1 << 6) | (1 << 7),
      HCI_PACKET_TYPE | ((len & 0x0f) << 4),
      (len & 0xff0) >> 4,
    ];
    h.push((~(h[0] + h[1] + h[2]) + 1) & 0xff);
    const raw = [...h, ...payload];
    const crc = crc16(raw);
    raw.push(crc & 0xff, crc >> 8);
    const out = [0xc0];
    for (const b of raw) {
      if (b === 0xc0) out.push(0xdb, 0xdc);
      else if (b === 0xdb) out.push(0xdb, 0xdd);
      else out.push(b);
    }
    out.push(0xc0);
    return Uint8Array.from(out);
  }
}

// The packets of one application update, in order, with the wait after each (seconds).
export function dfuPackets(bin, dat) {
  const pb = new PacketBuilder();
  const steps = [];
  const eraseWait = Math.max(0.5, (Math.floor(bin.length / FLASH_PAGE_SIZE) + 1) * FLASH_PAGE_ERASE_S);
  steps.push({ kind: 'start', pkt: pb.build([...u32(DFU_START_PACKET), ...u32(DFU_UPDATE_MODE_APP),
    ...u32(0), ...u32(0), ...u32(bin.length)]), wait: eraseWait });
  steps.push({ kind: 'init', pkt: pb.build([...u32(DFU_INIT_PACKET), ...dat, 0, 0]), wait: 0 });
  let count = 0;
  for (let i = 0; i < bin.length; i += DATA_PACKET_MAX, count++) {
    // after every 8 frames (one 4 KB page) the nRF blocks while it writes flash
    steps.push({ kind: 'data', pkt: pb.build([...u32(DFU_DATA_PACKET), ...bin.subarray(i, i + DATA_PACKET_MAX)]),
      wait: count % 8 === 0 ? FLASH_PAGE_WRITE_S : 0, done: Math.min(i + DATA_PACKET_MAX, bin.length) });
  }
  steps[steps.length - 1].wait += FLASH_PAGE_WRITE_S;
  steps.push({ kind: 'stop', pkt: pb.build(u32(DFU_STOP_DATA_PACKET)), wait: 0 });
  return steps;
}

// Single bank, application only: the bootloader still saves its settings (one page) after the port closes.
export const ACTIVATE_WAIT_S = FLASH_PAGE_ERASE_S + FLASH_PAGE_WRITE_S;

const sleep = (s) => new Promise((r) => setTimeout(r, s * 1000));

class AckReader {
  constructor(port) {
    this.buf = [];
    this.waiter = null;
    this.reader = port.readable.getReader();
    this.done = this.loop();
  }

  async loop() {
    try {
      for (;;) {
        const { value, done } = await this.reader.read();
        if (done) break;
        for (const b of value) this.buf.push(b);
        if (this.waiter) this.waiter();
      }
    } catch (e) {
      // port closed or device gone: getAck reports the timeout
    }
  }

  discard() { this.buf.length = 0; }

  // Bytes up to the second 0xC0 (one SLIP frame), or null after the timeout.
  async getAck() {
    const deadline = Date.now() + ACK_TIMEOUT_MS;
    for (;;) {
      const first = this.buf.indexOf(0xc0);
      const second = first < 0 ? -1 : this.buf.indexOf(0xc0, first + 1);
      if (second > 0) {
        const frame = this.buf.splice(0, second + 1).slice(first + 1, second);
        if (frame.length === 0) {           // two delimiters back to back: keep the second as a start
          this.buf.unshift(0xc0);
          continue;
        }
        return frame;
      }
      const left = deadline - Date.now();
      if (left <= 0) return null;
      await new Promise((r) => {
        this.waiter = r;
        setTimeout(r, left);
      });
      this.waiter = null;
    }
  }

  async close() {
    try { await this.reader.cancel(); } catch (e) { /* already gone */ }
    this.reader.releaseLock();
    await this.done;
  }
}

// Runs the whole update on an opened-by-us port. onProgress(doneBytes, totalBytes, phase).
export async function flash(port, bin, dat, onProgress = () => {}) {
  await port.open({ baudRate: 115200 });
  let acks = null;
  let writer = null;
  try {
    acks = new AckReader(port);
    writer = port.writable.getWriter();
    await sleep(0.1);
    // DTR asserted again = the KB Bridge resets the nRF into its bootloader (like a Feather's DTR)
    await port.setSignals({ dataTerminalReady: false });
    await sleep(0.05);
    await port.setSignals({ dataTerminalReady: true });
    await sleep(0.1);

    const steps = dfuPackets(bin, dat);
    onProgress(0, bin.length, 'erase');
    for (const s of steps) {
      // START erases the application area: no answer there means no bootloader, not a lost byte
      const attempts = s.kind === 'start' ? 1 : SEND_ATTEMPTS;
      let ack = null;
      for (let a = 0; a < attempts && ack === null; a++) {
        acks.discard();                     // a late ACK of an earlier try must not count for this one
        await writer.write(s.pkt);
        ack = await acks.getAck();
      }
      if (ack === null) {
        throw new Error(s.kind === 'start' ? 'NO_BOOTLOADER' : 'NO_ACK');
      }
      if (s.wait) await sleep(s.wait);
      if (s.kind === 'data') onProgress(s.done, bin.length, 'write');
    }
    onProgress(bin.length, bin.length, 'activate');
  } finally {
    if (writer) writer.releaseLock();
    if (acks) await acks.close();
    await port.close();
  }
  await sleep(ACTIVATE_WAIT_S);
}
