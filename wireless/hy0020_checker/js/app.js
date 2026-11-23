import { readDfuPackage } from './zip.js';
import { flash } from './dfu.js';

// CH340C on the checker board
const USB_FILTERS = [{ usbVendorId: 0x1a86, usbProductId: 0x7523 }];
const FIRMWARE = 'firmware/hy0020_checker_dfu.zip';

const TEXT = {
  ja: {
    lead: 'リフローした HY0020 SIP を HY0020 UART チェッカーに挿し、確認用ファームウェアを書き込んで動作を確かめるページです。',
    unsupported: 'このブラウザは Web Serial に対応していません。パソコンの Chrome か Edge で開いてください。',
    s1: '1. 確認用ファームウェアを書き込む',
    step1: 'SIP を「HY0020 を上」にして HY0020 UART チェッカーのコンスルーに挿し、USB-C をパソコンにつなぐ（D2 が点灯すれば電源 OK）',
    step2: '下の「書き込む」を押し、一覧から CH340 のポート（USB Serial）を選んで「接続」',
    step3: '「完了」と出たら、2. の「モニターを開く」で結果を見る。書き込みが始まらないときは、SW2 を押しながら SW1（RESET）を押してから、もう一度「書き込む」',
    localSummary: '手元のファイルを使う（上級者向け）',
    localHelp: 'fw/build.sh で作った hy0020_checker_dfu.zip などを選ぶと、同梱のファームウェアの代わりに書き込みます。',
    go: '書き込む',
    erase: '準備中（消去）…',
    write: '書き込み中… {p}%',
    activate: '仕上げ中…',
    done: '完了しました。2. の「モニターを開く」で確認できます。',
    NO_BOOTLOADER: 'ブートローダーから応答がありません。\nSW2 を押しながら SW1（RESET）を押して離し、もう一度「書き込む」を押してください。',
    NO_ACK: '書き込みの途中で応答が止まりました。\nもう一度書き込んでください。途中で止まっても、書き直せば元に戻ります。',
    BAD_PACKAGE: 'ファームウェアのファイルを読めませんでした。',
    FETCH: 'ファームウェアを読み込めませんでした。ページを再読み込みしてください。',
    OPEN: 'ポートを開けませんでした。ほかのアプリ（Arduino IDE やターミナル）が使っていないか確かめてください。',
    s2: '2. 確認する',
    monHelp: '確認用ファームウェアの出力（115200 bps）を表示します。開くと HY0020 がリセットされ、最初にデバイス ID などが出ます。',
    monOpen: 'モニターを開く',
    monClose: 'モニターを閉じる',
    cmdReport: '再表示',
    cmdReset: 'リセット',
    cmdDfu: 'DFU に入る',
    chkItem: '項目',
    chkOk: 'OK の目安',
    chkUart: '「HY0020 checker」とデバイス ID・VDD（3300 mV 前後）が表示され、「再表示」で表示し直せる',
    chkLed: '1 秒ごとに 2 回点滅する（出荷時のサンプルは 1 Hz の点滅）',
    chkSw: '押すと D1 が点灯し、「SW2 pressed」と表示される',
    chkBle: 'スマートフォンなどから「HY0020 XXXX」につなぐと「BLE connected」と RSSI が表示され、D1 が点灯する',
    footer: 'Chrome / Edge（Windows・macOS・Linux）で動きます。書き込みはすべてこのブラウザの中で行われ、ファイルや情報が外部に送られることはありません。',
  },
  en: {
    lead: 'Plug a reflowed HY0020 SIP into the HY0020 UART Checker, write the check firmware and see whether it works.',
    unsupported: 'This browser does not support Web Serial. Open this page in Chrome or Edge on a computer.',
    s1: '1. Write the check firmware',
    step1: 'Plug the SIP into the checker\'s ConThrough header with the HY0020 facing up and connect the USB-C to the computer (D2 lit = power OK)',
    step2: 'Press "Write" below, choose the CH340 port (USB Serial) and press "Connect"',
    step3: 'When it says "Done", use "Open monitor" in step 2 to see the result. If writing does not start, hold SW2, press SW1 (RESET), then press "Write" again',
    localSummary: 'Use a local file (advanced)',
    localHelp: 'Pick a hy0020_checker_dfu.zip made with fw/build.sh (or similar) to write it instead of the bundled firmware.',
    go: 'Write',
    erase: 'Preparing (erasing)…',
    write: 'Writing… {p}%',
    activate: 'Finishing…',
    done: 'Done. Check it with "Open monitor" in step 2.',
    NO_BOOTLOADER: 'No answer from the bootloader.\nHold SW2, press and release SW1 (RESET), then press "Write" again.',
    NO_ACK: 'The board stopped answering during the write.\nWrite again. An interrupted write is fixed by writing again.',
    BAD_PACKAGE: 'Could not read the firmware file.',
    FETCH: 'Could not load the firmware. Reload the page.',
    OPEN: 'Could not open the port. Check that no other app (Arduino IDE, a terminal) is using it.',
    s2: '2. Check it',
    monHelp: 'Shows the check firmware\'s output (115200 bps). Opening it resets the HY0020, which then prints its device id and more.',
    monOpen: 'Open monitor',
    monClose: 'Close monitor',
    cmdReport: 'Report',
    cmdReset: 'Reset',
    cmdDfu: 'Enter DFU',
    chkItem: 'Item',
    chkOk: 'OK when',
    chkUart: '"HY0020 checker", the device id and VDD (about 3300 mV) appear, and "Report" prints them again',
    chkLed: 'It double-blinks every second (the factory sample blinks at 1 Hz)',
    chkSw: 'Pressing it lights D1 and prints "SW2 pressed"',
    chkBle: 'Connecting to "HY0020 XXXX" from a phone or similar prints "BLE connected" and the RSSI, and D1 stays lit',
    footer: 'Works in Chrome / Edge on Windows, macOS and Linux. Everything happens inside this browser; no files or data are sent anywhere.',
  },
};

const $ = (id) => document.getElementById(id);
function savedLang() {
  try { return localStorage.getItem('lang'); } catch (e) { return null; }
}
let lang = (savedLang() || navigator.language || 'ja').startsWith('ja') ? 'ja' : 'en';
let localFile = null;
let port = null;            // the CH340 port chosen once, reused by the monitor
let mon = null;             // { reader, writer, done } while the monitor is open
let busy = false;
// Japanese text: a new line after every sentence (shown with white-space: pre-line).
const t = (key) => (TEXT[lang][key] || key).replace(/。(?![\n）」]|$)/g, '。\n');

function applyText() {
  document.documentElement.lang = lang;
  for (const el of document.querySelectorAll('[data-i18n]')) el.textContent = t(el.dataset.i18n);
  $('lang').textContent = lang === 'ja' ? 'English' : '日本語';
  $('mon').textContent = t(mon ? 'monClose' : 'monOpen');
}

function setStatus(text, cls = '') {
  $('status').textContent = text;
  $('status').className = 'status ' + cls;
}

function setBar(frac) {
  $('barfill').style.width = `${Math.round(frac * 100)}%`;
}

async function choosePort() {
  if (port) return port;
  port = await navigator.serial.requestPort({ filters: USB_FILTERS });
  return port;
}

async function loadPackage() {
  if (localFile) return readDfuPackage(await localFile.arrayBuffer());
  let res;
  try {
    res = await fetch(FIRMWARE, { cache: 'no-cache' });
  } catch (e) {
    throw new Error('FETCH');
  }
  if (!res.ok) throw new Error('FETCH');
  return readDfuPackage(await res.arrayBuffer());
}

async function run() {
  if (busy) return;
  try {
    await choosePort();
  } catch (e) {
    return;                                   // the user closed the chooser
  }
  busy = true;
  $('go').disabled = true;
  $('run').hidden = false;
  setBar(0);
  setStatus('');
  try {
    if (mon) await closeMonitor();
    let pkg;
    try {
      pkg = await loadPackage();
    } catch (e) {
      throw new Error(e.message === 'FETCH' ? 'FETCH' : 'BAD_PACKAGE');
    }
    await flash(port, pkg.bin, pkg.dat, (done, total, phase) => {
      setBar(done / total);
      setStatus(t(phase).replace('{p}', Math.floor((100 * done) / total)));
    });
    setBar(1);
    setStatus(t('done'), 'ok');
  } catch (e) {
    const known = ['NO_BOOTLOADER', 'NO_ACK', 'BAD_PACKAGE', 'FETCH'];
    const key = known.includes(e.message) ? e.message : e.name === 'NetworkError' || e.name === 'InvalidStateError' ? 'OPEN' : null;
    setStatus(key ? t(key) : `${t('NO_ACK')}\n(${e.name}: ${e.message})`, 'error');
  } finally {
    busy = false;
    $('go').disabled = false;
  }
}

// ---- serial monitor -----------------------------------------------------------------------
function log(text) {
  const el = $('log');
  el.textContent = (el.textContent + text).slice(-20000);
  el.scrollTop = el.scrollHeight;
}

function setMonitorUi(open) {
  $('mon').textContent = t(open ? 'monClose' : 'monOpen');
  for (const b of document.querySelectorAll('.cmd')) b.disabled = !open;
}

async function openMonitor() {
  try {
    await choosePort();
  } catch (e) {
    return;
  }
  try {
    await port.open({ baudRate: 115200 });
  } catch (e) {
    log(`\n[${t('OPEN')}]\n`);
    return;
  }
  // DTR edge = reset through C3, so the boot report shows up
  await port.setSignals({ dataTerminalReady: false });
  await new Promise((r) => setTimeout(r, 50));
  await port.setSignals({ dataTerminalReady: true });
  const reader = port.readable.getReader();
  const writer = port.writable.getWriter();
  const dec = new TextDecoder();
  const done = (async () => {
    try {
      for (;;) {
        const { value, done: end } = await reader.read();
        if (end) break;
        log(dec.decode(value, { stream: true }));
      }
    } catch (e) {
      // closed or unplugged
    }
  })();
  mon = { reader, writer, done };
  setMonitorUi(true);
}

async function closeMonitor() {
  if (!mon) return;
  const m = mon;
  mon = null;
  try { await m.reader.cancel(); } catch (e) { /* gone */ }
  m.reader.releaseLock();
  await m.done;
  m.writer.releaseLock();
  try { await port.close(); } catch (e) { /* gone */ }
  setMonitorUi(false);
}

async function send(cmd) {
  if (!mon) return;
  await mon.writer.write(new TextEncoder().encode(cmd));
  if (cmd === 'd') await closeMonitor();     // the bootloader takes over the port: write from step 1
}

function init() {
  if (!('serial' in navigator)) {
    $('unsupported').hidden = false;
    $('go').disabled = true;
    $('mon').disabled = true;
  }
  applyText();
  $('lang').addEventListener('click', () => {
    lang = lang === 'ja' ? 'en' : 'ja';
    try { localStorage.setItem('lang', lang); } catch (e) { /* private mode */ }
    applyText();
  });
  $('local').addEventListener('change', (e) => { localFile = e.target.files[0] || null; });
  $('go').addEventListener('click', run);
  $('mon').addEventListener('click', () => (mon ? closeMonitor() : openMonitor()));
  for (const b of document.querySelectorAll('.cmd')) b.addEventListener('click', () => send(b.dataset.cmd));
  navigator.serial?.addEventListener('disconnect', (e) => {
    if (e.target === port) { mon = null; setMonitorUi(false); port = null; }
  });
}

init();
