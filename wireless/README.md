# Wireless keyboards

Keyboards built from an STM32 (QMK / Vial) and a HY0020 (nRF52832) BLE module on its UART, and what they share. This folder is also the web site for them (GitHub Pages).

| Path | What |
| --- | --- |
| `index.html`, `js/`, `style.css` | firmware updater page for every keyboard here (Chrome / Edge: WebUSB for the STM32's ROM DFU, WebHID for the HY0020 through the keyboard) |
| `firmware/index.json`, `firmware/<keyboard>/` | the keyboards the page knows and their published firmware |
| `guide/` | build guides (`<keyboard>_ja.html`, `hy0020_checker_ja.html`) |
| `tools/add_firmware.py` | publishes a build on the updater page |
| `tools/test/` | tests of the page's protocol code against a fake keyboard and bootloader |
| `hy0020_checker/` | checker board for a reflowed HY0020 SIP: self-test firmware and its own flashing page |
| `keyboards/<keyboard>/` | each keyboard's firmware sources and tools (`keyboards/trident40_wireless/`: Trident40 Wireless) |

The QMK side of each keyboard lives in `../qmk/<keyboard>/`.

## Adding a keyboard

The updater page has no keyboard-specific code. A keyboard works with it when its STM32 firmware has the raw HID update tunnel (`hy0020_update.c` of `qmk/trident40_wireless`: commands 0xB0–0xB2 and 0xB5 on Vial's raw HID interface) and enters the STM32 ROM bootloader (0483:DF11).

1. Put its sources in `wireless/keyboards/<keyboard>/` and `qmk/<keyboard>/`.
2. Add it to `"keyboards"` in `firmware/index.json`: `id`, `name`, `usb` (VID / PID), `stm32` (flash address and page size), `keys` (how the update key and the boot button are called, per language) and optionally `guide`.
3. Add its build outputs to `KEYBOARDS` in `tools/add_firmware.py`, then run it to publish a build.

The page opens on a keyboard with `?kb=<id>` (and `&lang=ja` / `en`).

## Local test

```
python3 -m http.server 8017        # in this folder, then http://localhost:8017 in Chrome
tools/test/ref_packets.py <dfu.zip> ref.bin    # with adafruit-nrfutil's Python
node tools/test/test.mjs <dfu.zip> ref.bin <stm32.bin>
```

`js/zip.js` and `js/nrfdfu.js` come from [KB Bridge](https://github.com/darakuneko/kb_bridge) (MIT).
