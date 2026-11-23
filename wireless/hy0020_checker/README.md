# HY0020 UART Checker

Self-test firmware and a browser flashing page for a reflowed HY0020 SIP (HY0020 / nRF52832 SIP) on the hy0020_checker board.

- Page: `index.html` (Web Serial, Chrome / Edge). Writes `firmware/hy0020_checker_dfu.zip` through the board's CH340C (1A86:7523), the same serial DFU as `adafruit-nrfutil dfu serial --singlebank`, then shows the firmware's output in a serial monitor.
- Firmware: `fw/hy0020_checker/hy0020_checker.ino` (Arduino, Adafruit nRF52 core, board Feather nRF52832). `fw/build.sh` builds it and copies the DFU package to `firmware/`.

The firmware prints the device id, BLE address, VDD and chip temperature at 115200 bps, advertises as "HY0020 XXXX" (RSSI is printed once connected), double-blinks D1 (P0.07) while advertising and lights it while connected or while SW2 (P0.20) is pressed. Serial commands: `?` report, `d` serial DFU, `r` reset.

Local test: `python3 -m http.server 8001` in this folder, then open http://localhost:8001 in Chrome.

`js/dfu.js` and `js/zip.js` come from [KB Bridge](https://github.com/darakuneko/kb_bridge) (MIT).
