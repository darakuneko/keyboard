// Adafruit nRF52 legacy serial DFU packets (what `adafruit-nrfutil dfu serial --singlebank` sends).
// From KB Bridge (https://github.com/darakuneko/kb_bridge, MIT): the transport lives in hy0020.js.

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
