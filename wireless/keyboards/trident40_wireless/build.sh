#!/bin/bash
# Trident40 Wireless firmware build.
#   ./build.sh          HY0020 (nRF52832) -> build/trident40_ble_dfu.zip (adafruit-nrfutil package)
#   ./build.sh qmk      STM32 (vial-qmk)  -> build/trident40_wireless_vial.bin
# The QMK keyboard lives in vial-qmk/keyboards/trident40_wireless (VIAL_QMK, default ~/keeb/vial-qmk);
# qmk/trident40_wireless at the top of this repository is a copy of it, with
# qmk/trident40_wireless.zip for a single download (./build.sh qmk refreshes both).
set -euo pipefail
cd "$(dirname "$0")"
OUT=build
mkdir -p "$OUT"

if [ "${1:-}" = qmk ]; then
  VIAL_QMK=${VIAL_QMK:-~/keeb/vial-qmk}
  make -C "$VIAL_QMK" trident40_wireless:vial
  cp "$VIAL_QMK/.build/trident40_wireless_vial.bin" "$OUT"/
  rsync -a --delete "$VIAL_QMK/keyboards/trident40_wireless/" ../../../qmk/trident40_wireless/
  (cd ../../../qmk && rm -f trident40_wireless.zip && zip -rq -X trident40_wireless.zip trident40_wireless -x "*.DS_Store")
  echo "done: $OUT/trident40_wireless_vial.bin"
  exit 0
fi

FQBN=adafruit:nrf52:feather52832
NRFUTIL=${NRFUTIL:-$(command -v adafruit-nrfutil || echo ~/keeb/kb_bridge/fw/.venv/bin/adafruit-nrfutil)}
# Bigger UART RX buffer: loop() polls every 1 ms while the 1 Mbaud link may bring ~100 bytes.
FLAGS="${EXTRA_FLAGS:-}"
arduino-cli compile --fqbn "$FQBN" \
  --build-property "compiler.c.extra_flags=$FLAGS" \
  --build-property "compiler.cpp.extra_flags=$FLAGS" \
  --output-dir "$OUT/nrf" ble/trident40_ble
# application only; SoftDevice S132 6.1.1 is already on the HY0020
"$NRFUTIL" dfu genpkg --dev-type 0x0052 --sd-req 0x00B7 \
  --application "$OUT/nrf/trident40_ble.ino.hex" "$OUT/trident40_ble_dfu.zip"
echo "done: $OUT/trident40_ble_dfu.zip"
