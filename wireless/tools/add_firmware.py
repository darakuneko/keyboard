#!/usr/bin/env python3
"""Add a firmware release of a wireless keyboard to the updater page.

  tools/add_firmware.py trident40_wireless [--stm32 <vial.bin>] [--hy0020 <dfu.zip>] [--version 0.1.15]
                        [--notes "..."] [--notes-en "..."]

Copies the files to firmware/<keyboard>/ and puts the release at the top of firmware/index.json.
Defaults come from the keyboard's entry in KEYBOARDS below (the build outputs of its build.sh and
FW_VERSION_STR of its HY0020 firmware). A release may hold only one of the two files.

A new keyboard: add it to KEYBOARDS here and to "keyboards" in firmware/index.json (name, USB ids,
key names; see the README).
"""
import argparse
import datetime
import json
import os
import re
import shutil
import zipfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))     # wireless/ = the site
INDEX = os.path.join(ROOT, "firmware", "index.json")

KEYBOARDS = {
    "trident40_wireless": {
        "stm32": "keyboards/trident40_wireless/build/trident40_wireless_vial.bin",
        "hy0020": "keyboards/trident40_wireless/build/trident40_ble_dfu.zip",
        "version_file": "keyboards/trident40_wireless/ble/trident40_ble/config.h",
    },
}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("keyboard", choices=sorted(KEYBOARDS))
    ap.add_argument("--stm32")
    ap.add_argument("--hy0020")
    ap.add_argument("--version")
    ap.add_argument("--notes", default="")
    ap.add_argument("--notes-en", default="")
    a = ap.parse_args()
    kb = KEYBOARDS[a.keyboard]

    stm32 = a.stm32 or os.path.join(ROOT, kb["stm32"])
    hy0020 = a.hy0020 or os.path.join(ROOT, kb["hy0020"])
    version = a.version
    if not version:
        version = re.search(r'#define\s+FW_VERSION_STR\s+"([^"]+)"', open(os.path.join(ROOT, kb["version_file"])).read()).group(1)
    today = datetime.date.today()
    stamp = "%s_%s" % (version, today.strftime("%Y%m%d"))
    out_dir = os.path.join(ROOT, "firmware", a.keyboard)
    os.makedirs(out_dir, exist_ok=True)

    entry = {"version": version, "date": today.isoformat()}
    if os.path.exists(stm32):
        rel = "firmware/%s/%s_stm32_%s.bin" % (a.keyboard, a.keyboard, stamp)
        shutil.copyfile(stm32, os.path.join(ROOT, rel))
        entry["stm32"] = rel
    if os.path.exists(hy0020):
        z = zipfile.ZipFile(hy0020)
        app = json.loads(z.read("manifest.json"))["manifest"]["application"]
        z.getinfo(app["bin_file"]), z.getinfo(app["dat_file"])          # must be an application package
        rel = "firmware/%s/%s_hy0020_%s.zip" % (a.keyboard, a.keyboard, stamp)
        shutil.copyfile(hy0020, os.path.join(ROOT, rel))
        entry["hy0020"] = rel
    if "stm32" not in entry and "hy0020" not in entry:
        raise SystemExit("no firmware file found (build first)")
    if a.notes or a.notes_en:
        entry["notes"] = {"ja": a.notes or a.notes_en, "en": a.notes_en or a.notes}

    index = json.load(open(INDEX))
    board = next(k for k in index["keyboards"] if k["id"] == a.keyboard)
    board["firmware"] = [f for f in board.get("firmware", []) if (f["version"], f["date"]) != (entry["version"], entry["date"])]
    board["firmware"].insert(0, entry)
    with open(INDEX, "w") as f:
        json.dump(index, f, ensure_ascii=False, indent=2)
        f.write("\n")
    print("added", a.keyboard, version, [entry.get("stm32"), entry.get("hy0020")])


if __name__ == "__main__":
    main()
