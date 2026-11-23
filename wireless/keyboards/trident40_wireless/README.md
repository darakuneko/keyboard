# Trident40 Wireless firmware

A 40-key keyboard: USB when a cable is connected, BLE otherwise, powered by one CR2450. An STM32L432KB runs Vial (QMK); a HY0020 (nRF52832) on the HY0020 SIP daughter board does BLE and talks to the STM32 over a UART link.

- Build guide (Japanese): `../../guide/trident40_wireless_ja.html`
- Firmware updater page: `../../index.html` (WebUSB for the STM32, WebHID for the HY0020)

## Layout

| Path | What |
| --- | --- |
| `ble/trident40_ble/` | HY0020 firmware (Arduino, Adafruit nRF52 core 1.7.0, board Feather nRF52832) |
| `../../../qmk/trident40_wireless/` | STM32 firmware: the QMK keyboard, a copy of `keyboards/trident40_wireless` in vial-qmk |
| `tools/hy0020_update.py` | writes the HY0020 through the keyboard from a terminal (what the updater page does) |
| `tools/diag.py` | link, power and battery counters of a running keyboard (USB or BLE) |

## Build

```
./build.sh          # HY0020 -> build/trident40_ble_dfu.zip
./build.sh qmk      # STM32  -> build/trident40_wireless_vial.bin (vial-qmk in ~/keeb/vial-qmk or $VIAL_QMK)
```

Publish a build on the updater page: `../../tools/add_firmware.py trident40_wireless --notes "..."`.

## Writing from a terminal

```
dfu-util -a 0 -d 0483:df11 -s 0x08000000:leave -D build/trident40_wireless_vial.bin
python3 tools/hy0020_update.py build/trident40_ble_dfu.zip        # then press BT_UPDT
python3 tools/hy0020_update.py --erase-pairings                   # erase every pairing (USB only)
```

`hy0020_update.py` needs `hidapi` (`pip install hidapi`). Start it first and press BT_UPDT when it asks: the HY0020's bootloader waits for the first packet only for about 3 seconds.

The STM32 enters its ROM bootloader with BOOT1 held while plugging in USB (battery out), with QK_BOOT (Vial unlocked, USB connected), or from the updater page.
