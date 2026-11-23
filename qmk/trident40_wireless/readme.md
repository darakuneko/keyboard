# trident40_wireless

40-key keyboard, STM32L432KB + HY0020 (nRF52832) for BLE, CR2450 coin cell.

* Keyboard Maintainer: [Daraku-Neko](https://github.com/Daraku-Neko)
* Hardware Supported: Trident40 Wireless PCB

USB when a cable is connected, BLE otherwise. The HY0020 runs its own firmware (BLE HID + Vial
over BLE) and talks to the STM32 over USART1 (`link_proto.h`).

Make example for this keyboard (after setting up your build environment):

    make trident40_wireless:vial

## Bootloader

* **Bootmagic reset**: hold the top left key while plugging in USB
* **Physical reset button**: hold BOOT1 while plugging in USB (board out of the case)
* **Keycode in layout**: `QK_BOOT` (layer 3)

## Wireless keys (Vial custom keycodes, layer 3 in the default keymap)

| Key | Action |
| --- | --- |
| BT_1 .. BT_4 | BLE host 1-4, and output BLE (OUT_USB / OUT_AUTO go back to USB). Each host has its own BLE address, bonds and name ("Trident40 Wireless 1" .. "4"); the nRF keeps the selection through power-off |
| BT_PAIR | forget the selected host's bonds and advertise for a new one |
| OUT_AUTO / OUT_USB / OUT_BT | output: USB when connected else BLE / USB only / BLE only |
| BAT_LVL | type the battery level |
| BT_UPDT | HY0020 update mode (USB only; press again to leave) |
