// Firmware updater for the wireless keyboards in this repository: an STM32 (QMK / Vial) with a
// HY0020 (nRF52832) BLE module on its UART. The keyboards and their firmware come from
// firmware/index.json; nothing here is specific to one keyboard.
import { readDfuPackage } from './zip.js';
import { flashStm32, STM32_DFU_FILTERS } from './stm32dfu.js';
import { requestKeyboard, waitForUpdateMode, flashHy0020, CTRL_STM32_BOOT, CTRL_EXIT, CTRL_STATE, sleep } from './hy0020.js';

const TEXT = {
  ja: {
    lead: 'キーボードのファームウェアを、ブラウザから USB で書き込みます。ソフトのインストールは要りません。',
    unsupported: 'このブラウザは WebUSB / WebHID に対応していません。パソコンの Chrome か Edge で開いてください（Safari・Firefox・スマートフォンでは使えません）。',
    s1: '1. キーボードとファームウェアを選ぶ',
    keyboard: 'キーボード',
    version: 'バージョン',
    order: 'キーボード本体（STM32）と無線モジュール（HY0020）の 2 つを書き込みます。はじめて書き込むときは、2 → 3 の順に両方書き込んでください。更新のときも両方書き込みます（順番はどちらでも構いません）。',
    s2: '2. キーボード本体（STM32）',
    stm32Lead: 'キーの処理と USB を受け持つファームウェアです。書き込むと、キーマップは既定に戻ります（Vial で変更していた場合は、先に Vial で保存しておいてください）。',
    stm32Step1: 'キーボードを USB でつなぎ、書き込みモード（DFU）にする。次のどちらか',
    stm32StepA: 'ファームウェアが入っているとき: 「DFU モードにする」を押してキーボードを選び、表示が出たらキーボードの {update}を押す',
    stm32StepB: '何も入っていないとき・起動しないとき: 電池を外し、{boot}を押したまま USB をつなぐ',
    stm32Step2: '「書き込む」を押し、出てきた一覧から「STM32 BOOTLOADER」を選んで「接続」',
    stm32Step3: '「完了」と出るまで USB を抜かない（20 秒ほど）',
    stm32Boot: 'DFU モードにする',
    go: '書き込む',
    stm32Local: '.bin ファイルを選ぶと、上の一覧の代わりにそのファイルを書き込みます。',
    localSummary: '手元のファイルを使う（上級者向け）',
    s3: '3. 無線モジュール（HY0020）',
    nrfLead: 'BLE を受け持つファームウェアです。キーボード本体（STM32）を通して書き込むので、先に 2 を済ませておきます。',
    nrfStep1: 'キーボードを USB でつなぐ（Vial などキーボードの設定アプリは閉じる）',
    nrfStep2: '「書き込む」を押してキーボードを選び、表示が出たらキーボードの {update}を押す',
    nrfStep3: '「完了」と出るまで USB を抜かない（1 分ほど）',
    nrfLocal: 'DFU パッケージ（.zip）を選ぶと、上の一覧の代わりにそのファイルを書き込みます。',
    eraseLabel: '書き込んだあと、ペアリング情報を初期化する',
    eraseHelp: 'つながらなくなったときだけ使います。例えば、ペアリングし直しても電源を入れ直すと再接続されない、といったときです。すべてのペアリング情報が消えるので、接続先の機器からもキーボードを削除して、ペアリングし直してください。「初期化だけ行う」は、ファームウェアを書き込まずに初期化します。',
    eraseOnly: '初期化だけ行う',
    pressKey: 'キーボードの {update}を押してください…',
    pressKeyAgain: '書き込みモードをいったん終了しました。もう一度、キーボードの {update}を押してください…',
    bootSent: 'DFU モードにしました。\n続けて「書き込む」を押し、「STM32 BOOTLOADER」を選んでください。',
    erase: '準備中（消去しています）…',
    write: '書き込み中… {p}%',
    activate: '仕上げ中…',
    stm32Done: '完了しました。キーボードが再起動します。キーが打てるか確認してください。',
    nrfDone: '完了しました。',
    erasing: 'ペアリング情報を初期化しています…',
    eraseDone: 'ペアリング情報を初期化しました。\n接続先の機器からキーボードを削除して、ペアリングし直してください。',
    noFirmware: '（公開中のファームウェアはありません）',
    latest: '（最新）',
    guide: 'ビルドガイド',
    footer: 'Chrome / Edge（Windows・macOS・Linux）で動きます。Windows で「STM32 BOOTLOADER」が一覧に出ないときは、Zadig で WinUSB ドライバーを入れてください。書き込みはすべてこのブラウザの中で行われ、ファイルや情報が外部に送られることはありません。',
    NO_FILE: 'このバージョンには、このファームウェアが含まれていません。',
    FETCH: 'ファームウェアを読み込めませんでした。ページを再読み込みしてください。',
    BAD_PACKAGE: 'ファームウェアのファイルが壊れているか、形式が違います。',
    OPEN: 'キーボードを開けませんでした。Vial など、ほかのアプリがキーボードを使っていないか確かめてください。',
    GONE: 'キーボードとの接続が切れました。\nUSB でつないでいるほうのキーボードを選んでください（同じパソコンに BLE でもつながっていると、一覧に 2 つ出ます。BLE のほうは書き込みモードで切断されます）。',
    NO_KEY: 'キーが押されませんでした。もう一度やり直してください。',
    NOT_UPDATE: 'キーボードが応答しませんでした。キーボード本体（STM32）のファームウェアが古い可能性があります。Vial のロックを解除して QK_BOOT を押すか、電池を外して {boot}を押したまま USB をつなぎ、「書き込む」を押してください。',
    NO_BOOTLOADER: '無線モジュールから応答がありません。\nもう一度「書き込む」を押してやり直してください。',
    NO_ACK: '書き込みの途中で応答が止まりました。\nもう一度「書き込む」を押してやり直してください。途中で止まっても、書き直せば元に戻ります。',
    LEFT_UPDATE_MODE: '書き込みモードが終了しました（USB が抜けた、または {update} がもう一度押されました）。\nもう一度「書き込む」を押してやり直してください。',
    ERASE_FAIL: '初期化できませんでした。USB でパソコンにつないでいるか確かめて、もう一度やり直してください。',
    DFU: '書き込めませんでした。\nUSB をつないだまま、もう一度 DFU モードにしてやり直してください。Windows では Zadig で WinUSB ドライバーが必要です。',
  },
  en: {
    lead: 'Write keyboard firmware from your browser over USB. Nothing to install.',
    unsupported: 'This browser does not support WebUSB / WebHID. Open this page in Chrome or Edge on a computer (Safari, Firefox and phones do not work).',
    s1: '1. Choose the keyboard and firmware',
    keyboard: 'Keyboard',
    version: 'Version',
    order: 'There are two parts to write: the keyboard itself (STM32) and the wireless module (HY0020). The first time, write both, 2 then 3. For an update, write both as well (in either order).',
    s2: '2. Keyboard (STM32)',
    stm32Lead: 'The firmware that handles the keys and USB. Writing it puts the keymap back to the default (save your keymap in Vial first if you changed it).',
    stm32Step1: 'Connect the keyboard by USB and put it in DFU mode, one of:',
    stm32StepA: 'With firmware on it: press "Enter DFU mode", choose the keyboard, and when asked press {update} on the keyboard',
    stm32StepB: 'Empty or not starting: take the battery out, then hold {boot} while plugging in USB',
    stm32Step2: 'Press "Write", choose "STM32 BOOTLOADER" in the list and press "Connect"',
    stm32Step3: 'Keep USB plugged in until it says "Done" (about 20 s)',
    stm32Boot: 'Enter DFU mode',
    go: 'Write',
    stm32Local: 'Pick a .bin file to write it instead of the list above.',
    localSummary: 'Use a local file (advanced)',
    s3: '3. Wireless module (HY0020)',
    nrfLead: 'The firmware that handles BLE. It is written through the keyboard (STM32), so do step 2 first.',
    nrfStep1: 'Connect the keyboard by USB (close Vial and other keyboard apps)',
    nrfStep2: 'Press "Write", choose the keyboard, and when asked press {update} on the keyboard',
    nrfStep3: 'Keep USB plugged in until it says "Done" (about a minute)',
    nrfLocal: 'Pick a DFU package (.zip) to write it instead of the list above.',
    eraseLabel: 'Initialise the pairings after writing',
    eraseHelp: 'Only for when the keyboard no longer connects: for example, it does not reconnect after a power cycle even though you paired it again. Every pairing is erased, so remove the keyboard on your devices and pair again. "Initialise only" does it without writing firmware.',
    eraseOnly: 'Initialise only',
    pressKey: 'Press {update} on the keyboard…',
    pressKeyAgain: 'Update mode was ended. Press {update} on the keyboard once more…',
    bootSent: 'The keyboard is in DFU mode.\nNow press "Write" and choose "STM32 BOOTLOADER".',
    erase: 'Preparing (erasing)…',
    write: 'Writing… {p}%',
    activate: 'Finishing…',
    stm32Done: 'Done. The keyboard restarts.\nCheck that the keys type.',
    nrfDone: 'Done.',
    erasing: 'Initialising the pairings…',
    eraseDone: 'The pairings are erased.\nRemove the keyboard on your devices and pair again.',
    noFirmware: '(no firmware published)',
    latest: ' (latest)',
    guide: 'Build guide',
    footer: 'Works in Chrome / Edge on Windows, macOS and Linux. On Windows, if "STM32 BOOTLOADER" is not in the list, install the WinUSB driver with Zadig. Everything happens inside this browser; no files or data are sent anywhere.',
    NO_FILE: 'This version does not include this firmware.',
    FETCH: 'Could not load the firmware. Please reload the page.',
    BAD_PACKAGE: 'The firmware file is broken or has the wrong format.',
    OPEN: 'Could not open the keyboard. Make sure no other app (Vial, …) is using it.',
    GONE: 'The connection to the keyboard was lost.\nChoose the keyboard that is connected by USB (when it is also connected to this computer over BLE the list shows two; the BLE one disconnects in update mode).',
    NO_KEY: 'The key was not pressed. Please try again.',
    NOT_UPDATE: 'The keyboard did not answer. Its STM32 firmware may be an older one. Unlock Vial and press QK_BOOT, or take the battery out and hold {boot} while plugging in USB, then press "Write".',
    NO_BOOTLOADER: 'No answer from the wireless module.\nPress "Write" to try again.',
    NO_ACK: 'The wireless module stopped answering during the write.\nPress "Write" to try again. An interrupted write is fixed by writing again.',
    LEFT_UPDATE_MODE: 'Update mode ended (USB was unplugged, or {update} was pressed again).\nPress "Write" to try again.',
    ERASE_FAIL: 'Initialisation failed. Make sure the keyboard is connected to the computer by USB and try again.',
    DFU: 'The write failed.\nKeep USB plugged in, enter DFU mode again and retry. On Windows the WinUSB driver (Zadig) is needed.',
  },
};

const $ = (id) => document.getElementById(id);
function savedLang() {
  try { return localStorage.getItem('lang'); } catch (e) { return null; }
}
const params = new URLSearchParams(location.search);
const urlLang = params.get('lang');
let lang = (urlLang || savedLang() || navigator.language || 'ja').startsWith('ja') ? 'ja' : 'en';
let index = { keyboards: [] };
let busy = false;

function currentKeyboard() {
  return index.keyboards.find((k) => k.id === $('kb').value);
}

function currentFirmware() {
  const kb = currentKeyboard();
  return kb && kb.firmware[Number($('fw').value)];
}

// Text with the keyboard's own key names in it.
function t(key) {
  const kb = currentKeyboard();
  const name = (k) => (kb && kb.keys && kb.keys[k] && (kb.keys[k][lang] || kb.keys[k].ja)) || k;
  return breakLines((TEXT[lang][key] || key).replace('{update}', name('update')).replace('{boot}', name('boot')));
}

// Japanese text: a new line after every sentence (the elements that show it use white-space: pre-line).
function breakLines(text) {
  return text.replace(/。(?![\n）」]|$)/g, '。\n');
}

function applyText() {
  document.documentElement.lang = lang;
  for (const el of document.querySelectorAll('[data-i18n]')) el.textContent = t(el.dataset.i18n);
  $('lang').textContent = lang === 'ja' ? 'English' : '日本語';
  $('stm32StepA').textContent = t('stm32StepA');
  $('stm32StepB').textContent = t('stm32StepB');
  $('nrfStep2').textContent = t('nrfStep2');
  const kb = currentKeyboard();
  const guide = kb && kb.guide && (kb.guide[lang] || kb.guide.ja);
  $('guideItem').hidden = !guide;
  if (guide) {
    $('guide').href = guide;
    $('guide').textContent = `${kb.name} ${t('guide')}`;
  }
  document.title = kb ? `${kb.name} Firmware Updater` : 'Firmware Updater';
}

function fillKeyboards() {
  $('kb').innerHTML = '';
  for (const k of index.keyboards) $('kb').add(new Option(k.name, k.id));
  const want = params.get('kb');
  if (want && index.keyboards.some((k) => k.id === want)) $('kb').value = want;
  fillVersions();
}

function fillVersions() {
  const kb = currentKeyboard();
  $('fw').innerHTML = '';
  if (!kb || !kb.firmware.length) $('fw').add(new Option(TEXT[lang].noFirmware, ''));
  else kb.firmware.forEach((f, i) => $('fw').add(new Option(`${f.version} — ${f.date}${i === 0 ? TEXT[lang].latest : ''}`, String(i))));
  showNotes();
  applyText();
}

function showNotes() {
  const f = currentFirmware();
  $('notes').textContent = breakLines(f ? (f.notes && (f.notes[lang] || f.notes.ja)) || '' : '');
}

async function fetchFile(path) {
  if (!path) throw new Error('NO_FILE');
  let res;
  try {
    res = await fetch(path, { cache: 'no-cache' });
  } catch (e) {
    throw new Error('FETCH');
  }
  if (!res.ok) throw new Error('FETCH');
  return res.arrayBuffer();
}

async function loadStm32() {
  const local = $('stm32local').files[0];
  const f = currentFirmware();
  const buf = local ? await local.arrayBuffer() : await fetchFile(f && f.stm32);
  const bin = new Uint8Array(buf);
  // a Cortex-M image starts with its stack pointer (RAM) and reset vector (flash, thumb)
  const dv = new DataView(buf);
  if (bin.length < 8 || (dv.getUint32(0, true) >>> 24) !== 0x20 || (dv.getUint32(4, true) >>> 24) !== 0x08 || !(dv.getUint32(4, true) & 1)) {
    throw new Error('BAD_PACKAGE');
  }
  return bin;
}

async function loadHy0020() {
  const local = $('nrflocal').files[0];
  const f = currentFirmware();
  const buf = local ? await local.arrayBuffer() : await fetchFile(f && f.hy0020);
  try {
    return await readDfuPackage(buf);
  } catch (e) {
    throw new Error('BAD_PACKAGE');
  }
}

// One card's progress bar and status line.
function ui(prefix) {
  return {
    show() {
      $(`${prefix}run`).hidden = false;
      this.bar(0);
      this.status('');
    },
    bar(frac) { $(`${prefix}bar`).style.width = `${Math.round(frac * 100)}%`; },
    status(text, cls = '') {
      $(`${prefix}status`).textContent = text;
      $(`${prefix}status`).className = `status ${cls}`;
    },
    progress(done, total, phase) {
      this.bar(total ? done / total : 0);
      this.status(t(phase).replace('{p}', total ? Math.floor((100 * done) / total) : 0));
    },
    error(e, fallback) {
      const key = TEXT[lang][e.message] ? e.message : fallback;
      this.status(TEXT[lang][e.message] ? t(key) : `${t(key)}\n(${e.name}: ${e.message})`, 'error');
    },
  };
}

function setBusy(on) {
  busy = on;
  for (const id of ['stm32boot', 'stm32go', 'nrfgo', 'eraseonly', 'kb', 'fw']) $(id).disabled = on;
}

// "Enter DFU mode": update mode (a key press on the keyboard), then the STM32 jumps to its ROM bootloader.
async function stm32Boot() {
  if (busy) return;
  const kb = currentKeyboard();
  const u = ui('stm32');
  let tun;
  try {
    tun = await requestKeyboard(kb);
  } catch (e) {
    u.show();
    u.error(e, 'OPEN');
    return;
  }
  if (!tun) return;
  setBusy(true);
  u.show();
  try {
    await waitForUpdateMode(tun, (s) => u.status(t(s)), { fresh: false });
    await tun.ctrl(CTRL_STM32_BOOT);
    // the keyboard leaves USB when it jumps; one that stays does not know the request
    const deadline = Date.now() + 3000;
    while (!tun.gone && Date.now() < deadline) await sleep(100);
    if (!tun.gone) {
      await tun.ctrl(CTRL_EXIT);
      throw new Error('NOT_UPDATE');
    }
    u.status(t('bootSent'), 'ok');
  } catch (e) {
    u.error(e, 'OPEN');
  } finally {
    await tun.close();
    setBusy(false);
  }
}

async function stm32Write() {
  if (busy) return;
  const kb = currentKeyboard();
  const u = ui('stm32');
  let device;
  try {
    device = await navigator.usb.requestDevice({ filters: STM32_DFU_FILTERS });
  } catch (e) {
    return;                                   // the user closed the chooser
  }
  setBusy(true);
  u.show();
  try {
    const bin = await loadStm32();
    const opt = { address: Number(kb.stm32.address), pageSize: kb.stm32.page_size };
    await flashStm32(device, bin, opt, (done, total, phase) => {
      if (phase === 'erase') {
        u.bar(0);
        u.status(t('erase'));
      } else {
        u.progress(done, total, 'write');
      }
    });
    u.bar(1);
    u.status(t('stm32Done'), 'ok');
  } catch (e) {
    u.error(e, 'DFU');
  } finally {
    setBusy(false);
  }
}

async function erasePairings(tun, u) {
  u.status(t('erasing'));
  if (await tun.ctrl(CTRL_STATE)) await tun.ctrl(CTRL_EXIT);   // update mode holds the link: give it back
  await sleep(2000);                                            // the HY0020 application starts
  if (!(await tun.erasePairings())) throw new Error('ERASE_FAIL');
}

async function nrfWrite(writeFirmware) {
  if (busy) return;
  const kb = currentKeyboard();
  const u = ui('nrf');
  let tun;
  try {
    tun = await requestKeyboard(kb);
  } catch (e) {
    u.show();
    u.error(e, 'OPEN');
    return;
  }
  if (!tun) return;
  setBusy(true);
  u.show();
  try {
    if (writeFirmware) {
      const pkg = await loadHy0020();
      await waitForUpdateMode(tun, (s) => u.status(t(s)));
      await flashHy0020(tun, pkg.bin, pkg.dat, (done, total, phase) => u.progress(done, total, phase));
    }
    const erase = !writeFirmware || $('erase').checked;
    if (erase) await erasePairings(tun, u);
    u.bar(1);
    u.status(writeFirmware ? t('nrfDone') + (erase ? `\n${t('eraseDone')}` : '') : t('eraseDone'), 'ok');
  } catch (e) {
    u.error(e, 'NO_ACK');
  } finally {
    await tun.close();
    setBusy(false);
  }
}

async function init() {
  if (!('usb' in navigator) || !('hid' in navigator)) {
    $('unsupported').hidden = false;
    for (const id of ['stm32boot', 'stm32go', 'nrfgo', 'eraseonly']) $(id).disabled = true;
  }
  try {
    const res = await fetch('firmware/index.json', { cache: 'no-cache' });
    index = await res.json();
    for (const k of index.keyboards) k.usb = { vid: Number(k.usb.vid), pid: Number(k.usb.pid) };
  } catch (e) {
    index = { keyboards: [] };
  }
  fillKeyboards();
  $('lang').addEventListener('click', () => {
    lang = lang === 'ja' ? 'en' : 'ja';
    try { localStorage.setItem('lang', lang); } catch (e) { /* private mode */ }
    const keep = $('fw').value;
    fillVersions();
    if (keep) $('fw').value = keep;
    showNotes();
  });
  $('kb').addEventListener('change', fillVersions);
  $('fw').addEventListener('change', showNotes);
  $('stm32boot').addEventListener('click', stm32Boot);
  $('stm32go').addEventListener('click', stm32Write);
  $('nrfgo').addEventListener('click', () => nrfWrite(true));
  $('eraseonly').addEventListener('click', () => nrfWrite(false));
}

init();
