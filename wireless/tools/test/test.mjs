// node tools/test/test.mjs <dfu.zip> <ref_packets.bin> <stm32.bin>
// 1. the HY0020 packets match the Python tool's byte for byte, sent through a fake keyboard
// 2. the STM32 DfuSe sequence against a fake bootloader: flash contents equal the image
import { readFileSync } from 'node:fs';
import { readDfuPackage } from '../../js/zip.js';
import { Tunnel, waitForUpdateMode, flashHy0020 } from '../../js/hy0020.js';
import { flashStm32, pageSizeFromName, transferSizeFromConfig } from '../../js/stm32dfu.js';

let failed = 0;
const check = (name, ok) => { console.log(ok ? 'ok  ' : 'FAIL', name); if (!ok) failed++; };

// ---- fake keyboard (raw HID): update mode starts on the 3rd state poll; the "bootloader" ACKs each SLIP frame
class FakeHid extends EventTarget {
  constructor() { super(); this.opened = false; this.active = false; this.polls = 0; this.toNrf = []; this.frames = 0; this.dropAck = 5; }
  async open() { this.opened = true; }
  async close() { this.opened = false; }
  report(bytes) {
    const d = new Uint8Array(32); d.set(bytes);
    const e = new Event('inputreport'); e.data = new DataView(d.buffer);
    setTimeout(() => this.dispatchEvent(e), 0);
  }
  async sendReport(id, data) {
    const d = Array.from(data);
    if (d[0] === 0xb2) {
      if (d[1] === 0 && ++this.polls >= 3) this.active = true;
      if (d[1] === 2) this.active = false;
      this.report([0xb2, d[1], this.active ? 1 : 0]);
    } else if (d[0] === 0xb0) {
      const chunk = d.slice(2, 2 + d[1]);
      this.report([0xb0, this.active ? d[1] : 0]);
      for (const b of chunk) {
        if (!this.frame) { if (b === 0xc0) this.frame = [b]; continue; }
        this.frame.push(b);
        if (b !== 0xc0) continue;
        // drop one ACK on purpose: the sender must repeat that packet, which is then not counted twice
        if (++this.frames === this.dropAck) { /* lost */ }
        else { this.toNrf.push(...this.frame); this.report([0xb1, 4, 0xc0, 0x11, 0x22, 0xc0]); }
        this.frame = null;
      }
    } else if (d[0] === 0xb5) {
      this.report([0xb5, this.active ? 0 : 1]);
    }
  }
}
Object.defineProperty(globalThis, 'navigator', { value: { hid: new EventTarget() }, configurable: true });
const realTimeout = globalThis.setTimeout;
globalThis.setTimeout = (f, ms) => realTimeout(f, Math.min(ms || 0, 2));    // no real waiting

const zip = readFileSync(process.argv[2]);
const { bin, dat } = await readDfuPackage(zip.buffer.slice(zip.byteOffset, zip.byteOffset + zip.byteLength));
const hid = new FakeHid();
const tun = new Tunnel(hid);
await tun.open();
const states = [];
await waitForUpdateMode(tun, (s) => states.push(s));
check('asks for the key', states.join() === 'pressKey' && hid.active);
let last = 0;
await flashHy0020(tun, bin, dat, (done) => { last = done; });
const ref = readFileSync(process.argv[3]);
check(`HY0020 packets match the Python reference (${ref.length} bytes, one ACK dropped and resent)`, Buffer.from(hid.toNrf).equals(ref));
check('progress reached the end, update mode left', last === bin.length && !hid.active);
check('erase pairings', await tun.erasePairings(100));

// ---- fake STM32 ROM bootloader
class FakeUsb {
  constructor() {
    this.flash = new Uint8Array(128 * 1024).fill(0x00);        // "dirty": erased pages become 0xff
    this.ptr = 0; this.state = 10; this.left = false; this.log = [];
    this.configuration = null;
  }
  async open() {}
  async close() {}
  async selectConfiguration() {
    this.configuration = { interfaces: [{ interfaceNumber: 0, alternates: [
      { alternateSetting: 0, interfaceClass: 0xfe, interfaceSubclass: 1, interfaceName: '@Internal Flash  /0x08000000/128*0002Kg' },
      { alternateSetting: 1, interfaceClass: 0xfe, interfaceSubclass: 1, interfaceName: '@Option Bytes  /0x1FFF7800/01*040 e' }] }] };
  }
  async claimInterface() {}
  async selectAlternateInterface(n, alt) { this.alt = alt; }
  async controlTransferIn(setup, len) {
    if (setup.requestType === 'standard') {
      const cfg = [9, 2, 27, 0, 1, 1, 0, 0xc0, 50, 9, 4, 0, 0, 0, 0xfe, 1, 2, 4, 9, 0x21, 0x0b, 0xff, 0, 0x00, 0x08, 0x1a, 0x01];
      return { status: 'ok', data: new DataView(Uint8Array.from(cfg).buffer) };
    }
    if (this.state === 4) this.state = 5;                       // busy once, then done
    else if (this.pending) { this.pending(); this.pending = null; this.state = 4; }
    return { status: 'ok', data: new DataView(Uint8Array.from([this.state === 10 ? 1 : 0, 1, 0, 0, this.state, 0]).buffer) };
  }
  async controlTransferOut(setup, data) {
    const d = data ? new Uint8Array(data.buffer || data, data.byteOffset || 0, data.byteLength) : new Uint8Array(0);
    if (setup.request === 4) { this.state = 2; return { status: 'ok' }; }
    if (setup.request === 6) { this.state = 2; return { status: 'ok' }; }
    if (setup.request !== 1) throw new Error('unexpected request');
    if (setup.value === 0 && d.length === 5) {
      const a = (d[1] | (d[2] << 8) | (d[3] << 16) | (d[4] << 24)) >>> 0;
      if (d[0] === 0x21) this.pending = () => { this.ptr = a; };
      else if (d[0] === 0x41) this.pending = () => { this.flash.fill(0xff, a - 0x08000000, a - 0x08000000 + 2048); this.log.push('E' + (a - 0x08000000) / 2048); };
      else throw new Error('unknown command');
    } else if (d.length === 0) {
      this.pending = () => { this.left = this.ptr; };
    } else {
      const copy = Uint8Array.from(d);
      const at = this.ptr + (setup.value - 2) * 2048 - 0x08000000;
      this.pending = () => { for (let i = 0; i < copy.length; i++) this.flash[at + i] &= copy[i]; };   // flash only clears bits
    }
    return { status: 'ok' };
  }
}
check('page size from the layout string', pageSizeFromName('@Internal Flash  /0x08000000/128*0002Kg', 0x08000000) === 2048
  && pageSizeFromName('@Internal Flash  /0x08000000/04*016Kg,01*064Kg,07*128Kg', 0x08020000) === 131072);
check('transfer size from the descriptor', transferSizeFromConfig(Uint8Array.from([9, 2, 0, 0, 0, 0, 0, 0, 0, 9, 0x21, 0x0b, 0xff, 0, 0x00, 0x08, 0x1a, 0x01])) === 2048);
const img = new Uint8Array(readFileSync(process.argv[4]));
const usb = new FakeUsb();
let wrote = 0;
await flashStm32(usb, img, { address: 0x08000000, pageSize: 2048 }, (done, total, phase) => { if (phase === 'write') wrote = done / total; });
check('STM32 flash holds the image', Buffer.from(usb.flash.subarray(0, img.length)).equals(Buffer.from(img)));
check('only the pages the image covers were erased', usb.log.length === Math.ceil(img.length / 2048) && usb.flash[Math.ceil(img.length / 2048) * 2048] === 0x00);
check('left DFU at the start address, progress 100 %', usb.left === 0x08000000 && wrote === 1 && usb.alt === 0);
process.exit(failed ? 1 : 0);
