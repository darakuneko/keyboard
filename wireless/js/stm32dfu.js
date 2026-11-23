// STM32 ROM bootloader (USB DFU with ST's DfuSe extension, 0483:DF11) over WebUSB: erase the pages
// the image covers, write it, leave DFU. What `dfu-util -a 0 -s 0x08000000:leave -D file.bin` does.

export const STM32_DFU_FILTERS = [{ vendorId: 0x0483, productId: 0xdf11 }];

const DNLOAD = 1;
const GETSTATUS = 3;
const CLRSTATUS = 4;
const ABORT = 6;

const STATE_IDLE = 2;
const STATE_DNBUSY = 4;
const STATE_DNLOAD_IDLE = 5;
const STATE_ERROR = 10;

const CMD_SET_ADDRESS = 0x21;
const CMD_ERASE = 0x41;

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));
const u32 = (v) => [v & 0xff, (v >>> 8) & 0xff, (v >>> 16) & 0xff, (v >>> 24) & 0xff];

// "@Internal Flash  /0x08000000/128*0002Kg" -> page size in bytes (the segment that holds `address`).
export function pageSizeFromName(name, address) {
  if (!name) return null;
  const m = name.match(/\/0x([0-9a-fA-F]+)\/(.+)$/);
  if (!m) return null;
  let start = parseInt(m[1], 16);
  for (const seg of m[2].split(',')) {
    const s = seg.match(/(\d+)\*(\d+)([ BKM])/);
    if (!s) return null;
    const size = Number(s[2]) * (s[3] === 'K' ? 1024 : s[3] === 'M' ? 1024 * 1024 : 1);
    const end = start + Number(s[1]) * size;
    if (address >= start && address < end) return size;
    start = end;
  }
  return null;
}

// wTransferSize of the DFU functional descriptor (type 0x21) in a configuration descriptor.
export function transferSizeFromConfig(bytes) {
  for (let i = 0; i + 1 < bytes.length && bytes[i] >= 2; i += bytes[i]) {
    if (bytes[i + 1] === 0x21 && bytes[i] >= 7) return bytes[i + 5] | (bytes[i + 6] << 8);
  }
  return null;
}

class Dfu {
  constructor(device, iface) {
    this.dev = device;
    this.iface = iface;
  }

  async out(request, value, data) {
    const r = await this.dev.controlTransferOut(
      { requestType: 'class', recipient: 'interface', request, value, index: this.iface }, data);
    if (r.status !== 'ok') throw new Error('DFU_TRANSFER');
  }

  async status() {
    const r = await this.dev.controlTransferIn(
      { requestType: 'class', recipient: 'interface', request: GETSTATUS, value: 0, index: this.iface }, 6);
    if (r.status !== 'ok' || r.data.byteLength < 6) throw new Error('DFU_TRANSFER');
    const d = r.data;
    return { status: d.getUint8(0), poll: d.getUint8(1) | (d.getUint8(2) << 8) | (d.getUint8(3) << 16), state: d.getUint8(4) };
  }

  // To dfuIDLE, whatever a previous tool left behind.
  async toIdle() {
    for (let i = 0; i < 5; i++) {
      const s = await this.status();
      if (s.state === STATE_IDLE) return;
      if (s.state === STATE_ERROR) await this.out(CLRSTATUS, 0);
      else await this.out(ABORT, 0);
    }
    throw new Error('DFU_STATE');
  }

  // One download block and the wait the device asks for.
  async download(block, data) {
    await this.out(DNLOAD, block, data);
    let s = await this.status();                   // starts the operation
    while (s.state === STATE_DNBUSY) {
      await sleep(s.poll);
      s = await this.status();
    }
    if (s.state !== STATE_DNLOAD_IDLE || s.status !== 0) throw new Error('DFU_STATE');
  }

  command(cmd, address) {
    return this.download(0, Uint8Array.from([cmd, ...u32(address)]));
  }
}

// device: a USBDevice the user picked (STM32_DFU_FILTERS). opt: { address, pageSize } (pageSize is
// only used when the bootloader's memory layout string cannot be read).
// onProgress(done, total, phase): phase 'erase' (pages) or 'write' (bytes).
export async function flashStm32(device, bin, opt, onProgress = () => {}) {
  await device.open();
  try {
    if (device.configuration === null) await device.selectConfiguration(1);
    let found = null;
    for (const itf of device.configuration.interfaces) {
      for (const alt of itf.alternates) {
        if (alt.interfaceClass !== 0xfe || alt.interfaceSubclass !== 1) continue;
        const flash = /Internal Flash/i.test(alt.interfaceName || '');
        if (!found || (flash && !found.flash)) found = { itf, alt, flash };
      }
    }
    if (!found) throw new Error('DFU_NO_INTERFACE');
    const ifaceNum = found.itf.interfaceNumber;
    await device.claimInterface(ifaceNum);
    await device.selectAlternateInterface(ifaceNum, found.alt.alternateSetting);

    let transfer = null;
    try {
      const r = await device.controlTransferIn(
        { requestType: 'standard', recipient: 'device', request: 6, value: 0x0200, index: 0 }, 255);
      if (r.status === 'ok') transfer = transferSizeFromConfig(new Uint8Array(r.data.buffer, r.data.byteOffset, r.data.byteLength));
    } catch (e) { /* default below */ }
    if (!transfer || transfer > 2048) transfer = 2048;
    const page = pageSizeFromName(found.alt.interfaceName, opt.address) || opt.pageSize || 2048;

    const dfu = new Dfu(device, ifaceNum);
    await dfu.toIdle();

    const pages = Math.ceil(bin.length / page);
    for (let i = 0; i < pages; i++) {
      onProgress(i, pages, 'erase');
      await dfu.command(CMD_ERASE, opt.address + i * page);
    }

    // the flash is programmed in 8-byte words: pad the image
    const image = new Uint8Array(Math.ceil(bin.length / 8) * 8).fill(0xff);
    image.set(bin);
    await dfu.command(CMD_SET_ADDRESS, opt.address);
    for (let off = 0, block = 2; off < image.length; off += transfer, block++) {
      await dfu.download(block, image.subarray(off, off + transfer));   // address + (block - 2) * transfer
      onProgress(Math.min(off + transfer, image.length), image.length, 'write');
    }

    // leave: the address to start from, then an empty download; the device disconnects and runs it
    await dfu.command(CMD_SET_ADDRESS, opt.address);
    try {
      await dfu.out(DNLOAD, 0, new Uint8Array(0));
      await dfu.status();
    } catch (e) { /* it is already gone */ }
  } finally {
    try { await device.close(); } catch (e) { /* gone */ }
  }
}
