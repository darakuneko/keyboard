// Minimal ZIP reader for the DFU package (stored or deflated entries), using DecompressionStream.

async function inflateRaw(data) {
  const stream = new Blob([data]).stream().pipeThrough(new DecompressionStream('deflate-raw'));
  return new Uint8Array(await new Response(stream).arrayBuffer());
}

// Returns { name: Uint8Array } for every file in the archive.
export async function unzip(buffer) {
  const u8 = new Uint8Array(buffer);
  const dv = new DataView(buffer);
  let eocd = -1;
  for (let i = u8.length - 22; i >= Math.max(0, u8.length - 22 - 0xffff); i--) {
    if (dv.getUint32(i, true) === 0x06054b50) { eocd = i; break; }
  }
  if (eocd < 0) throw new Error('BAD_ZIP');
  const count = dv.getUint16(eocd + 10, true);
  let p = dv.getUint32(eocd + 16, true);
  const files = {};
  const dec = new TextDecoder();
  for (let n = 0; n < count; n++) {
    if (dv.getUint32(p, true) !== 0x02014b50) throw new Error('BAD_ZIP');
    const method = dv.getUint16(p + 10, true);
    const csize = dv.getUint32(p + 20, true);
    const nameLen = dv.getUint16(p + 28, true);
    const extraLen = dv.getUint16(p + 30, true);
    const commentLen = dv.getUint16(p + 32, true);
    const local = dv.getUint32(p + 42, true);
    const name = dec.decode(u8.subarray(p + 46, p + 46 + nameLen));
    const dataStart = local + 30 + dv.getUint16(local + 26, true) + dv.getUint16(local + 28, true);
    const raw = u8.subarray(dataStart, dataStart + csize);
    if (method === 0) files[name] = raw.slice();
    else if (method === 8) files[name] = await inflateRaw(raw);
    else throw new Error('BAD_ZIP');
    p += 46 + nameLen + extraLen + commentLen;
  }
  return files;
}

// The application image and init packet of an adafruit-nrfutil package.
export async function readDfuPackage(buffer) {
  const files = await unzip(buffer);
  const manifest = JSON.parse(new TextDecoder().decode(files['manifest.json'] || new Uint8Array()));
  const app = manifest.manifest && manifest.manifest.application;
  if (!app || !files[app.bin_file] || !files[app.dat_file]) throw new Error('BAD_PACKAGE');
  return { bin: files[app.bin_file], dat: files[app.dat_file] };
}
