#!/bin/bash
# HY0020 checker firmware: arduino-cli -> DFU package for adafruit-nrfutil (SoftDevice S132 6.1.1 is on the HY0020).
#   ./build.sh     outputs build/hy0020_checker_dfu.zip
# Write it on the checker board (DTR resets into the bootloader; if not, hold SW2 and press SW1):
#   adafruit-nrfutil --verbose dfu serial -pkg build/hy0020_checker_dfu.zip -p <port> -b 115200 --singlebank
set -euo pipefail
cd "$(dirname "$0")"
FQBN=adafruit:nrf52:feather52832
OUT=build
NRFUTIL=${NRFUTIL:-$(command -v adafruit-nrfutil || echo ~/keeb/kb_bridge/fw/.venv/bin/adafruit-nrfutil)}
mkdir -p "$OUT"
arduino-cli compile --fqbn "$FQBN" --output-dir "$OUT/nrf" hy0020_checker
"$NRFUTIL" dfu genpkg --dev-type 0x0052 --sd-req 0x00B7 \
  --application "$OUT/nrf/hy0020_checker.ino.hex" "$OUT/hy0020_checker_dfu.zip"
cp "$OUT/hy0020_checker_dfu.zip" ../firmware/   # the browser page writes this one
echo "done: $OUT/hy0020_checker_dfu.zip (copied to ../firmware/)"
