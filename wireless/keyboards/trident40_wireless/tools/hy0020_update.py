#!/usr/bin/env python3
"""Write the HY0020 (nRF52832) application on a Trident40 Wireless over USB.

The STM32 tunnels the nRF bootloader's serial DFU through Vial's raw HID interface (see
hy0020_update.c in the QMK keyboard). This does what `adafruit-nrfutil dfu serial --singlebank`
does, over that tunnel.

  pip install hidapi
  python3 tools/hy0020_update.py build/trident40_ble_dfu.zip

Press BT_UPDT on the keyboard (layer 3 in the default keymap) first; the script waits for it.

  python3 tools/hy0020_update.py --erase-pairings [zip]

erases every BLE pairing on the HY0020 (its file system is formatted; after an update when a zip is
given). For when pairings no longer stick across a power cycle: a reset or power loss in the middle
of a flash write can leave the HY0020's file system corrupt, and writing the firmware again does not
fix that. Not in update mode: the HY0020 application takes the command (same as holding BT_CLR).
Update mode asks the HY0020 firmware to restart in its bootloader (serial DFU). For other firmware
on the HY0020, --enter sends its own command first, e.g. --enter d for the hy0020_checker firmware.
"""
import argparse
import json
import sys
import time
import zipfile

import hid

VID, PID = 0x7A79, 0x1577
RAW_USAGE_PAGE = 0xFF60

UPD_WRITE, UPD_READ, UPD_CTRL, UPD_ERASE = 0xB0, 0xB1, 0xB2, 0xB5
CTRL_STATE, CTRL_DFU, CTRL_EXIT, CTRL_BAUD = 0, 1, 2, 3
BOOTLOADER_BAUD = 115200
# HY0020 application link (link_proto.h): BT_CMD_ENTER_DFU makes it restart in the bootloader.
LINK_SOF, MSG_BT_CMD, BT_CMD_ENTER_DFU = 0x7E, 0x21, 3
# After a pin reset the bootloader listens for serial DFU only briefly: START goes out this long
# after the reset (adafruit-nrfutil: 0.1 s after DTR), and the whole attempt is repeated if it is missed.
RESET_TO_START_S = 0.1
ENTER_ATTEMPTS = 5

# Adafruit nRF52 legacy serial DFU (nordicsemi/dfu/dfu_transport_serial.py)
DFU_INIT_PACKET, DFU_START_PACKET, DFU_DATA_PACKET, DFU_STOP_DATA_PACKET = 1, 3, 4, 5
DFU_UPDATE_MODE_APP = 4
HCI_PACKET_TYPE = 14
DATA_PACKET_MAX = 512
FLASH_PAGE_SIZE = 4096
FLASH_PAGE_ERASE_S = 0.0897
FLASH_PAGE_WRITE_S = (FLASH_PAGE_SIZE / 4) * 0.0001
ACK_TIMEOUT_S = 1.0
SEND_ATTEMPTS = 4  # the bootloader drops a duplicate sequence number and only acknowledges it


def crc16(data, crc=0xFFFF):
    for b in data:
        crc = ((crc >> 8) & 0xFF) | ((crc << 8) & 0xFF00)
        crc ^= b
        crc ^= (crc & 0xFF) >> 4
        crc ^= (crc << 12) & 0xFFFF
        crc ^= ((crc & 0xFF) << 5) & 0xFFFF
    return crc & 0xFFFF


def u32(v):
    return list(v.to_bytes(4, 'little'))


class PacketBuilder:
    def __init__(self):
        self.seq = 0

    def build(self, payload):
        self.seq = (self.seq + 1) % 8
        n = len(payload)
        h = [self.seq | (((self.seq + 1) % 8) << 3) | (1 << 6) | (1 << 7),
             HCI_PACKET_TYPE | ((n & 0x0F) << 4), (n & 0xFF0) >> 4]
        h.append((~sum(h) + 1) & 0xFF)
        raw = h + list(payload)
        c = crc16(raw)
        raw += [c & 0xFF, c >> 8]
        out = [0xC0]
        for b in raw:
            out += [0xDB, 0xDC] if b == 0xC0 else [0xDB, 0xDD] if b == 0xDB else [b]
        return bytes(out + [0xC0])


def dfu_steps(bin_, dat):
    pb = PacketBuilder()
    erase_wait = max(0.5, (len(bin_) // FLASH_PAGE_SIZE + 1) * FLASH_PAGE_ERASE_S)
    steps = [('start', pb.build(u32(DFU_START_PACKET) + u32(DFU_UPDATE_MODE_APP) + u32(0) + u32(0) + u32(len(bin_))), erase_wait, 0),
             ('init', pb.build(u32(DFU_INIT_PACKET) + list(dat) + [0, 0]), 0, 0)]
    for count, i in enumerate(range(0, len(bin_), DATA_PACKET_MAX)):
        wait = FLASH_PAGE_WRITE_S if count % 8 == 0 else 0  # one 4 KB page every 8 packets
        steps.append(('data', pb.build(u32(DFU_DATA_PACKET) + list(bin_[i:i + DATA_PACKET_MAX])), wait,
                      min(i + DATA_PACKET_MAX, len(bin_))))
    k, p, w, d = steps[-1]
    steps[-1] = (k, p, w + FLASH_PAGE_WRITE_S, d)
    steps.append(('stop', pb.build(u32(DFU_STOP_DATA_PACKET)), 0, 0))
    return steps


class Tunnel:
    def __init__(self):
        self.dev = None
        self.rx = bytearray()
        self.open()

    def open(self, wait_s=120):
        """(Re)opens the keyboard. It re-enumerates when the USB cable is plugged in (VBUS)."""
        if self.dev:
            self.dev.close()
        deadline = time.time() + wait_s
        while True:
            # USB only (interface_number >= 0): the keyboard is also a HID device over BLE, with the
            # same ids, and update mode drops the BLE link.
            path = next((d['path'] for d in hid.enumerate(VID, PID)
                         if d['usage_page'] == RAW_USAGE_PAGE and d['interface_number'] >= 0), None)
            if path:
                try:
                    self.dev = hid.device()
                    self.dev.open_path(path)
                    return
                except OSError:
                    pass
            if time.time() > deadline:
                sys.exit('Trident40 Wireless not found on USB (cable connected? Vial closed?)')
            time.sleep(0.5)

    def _read(self, timeout_ms):
        try:
            return self.dev.read(32, timeout_ms)
        except OSError:
            self.open()
            return None

    def _send(self, report):
        self.dev.write(bytes([0]) + bytes(report).ljust(32, b'\0'))

    def _answer(self, cmd, timeout=1.0):
        """Reads reports until the answer to `cmd`; tunnel data that comes meanwhile is kept."""
        deadline = time.time() + timeout
        while time.time() < deadline:
            r = self._read(50)
            if not r:
                continue
            if r[0] == UPD_READ:
                self.rx += bytes(r[2:2 + r[1]])
            elif r[0] == cmd:
                return r
        return None

    def ctrl(self, op, timeout=1.0):
        self._send([UPD_CTRL, op])
        r = self._answer(UPD_CTRL, timeout)
        return bool(r and r[2])

    def erase_pairings(self, wait_s=15):
        """Erase every pairing (BT_CMD_ERASE_ALL); retried while the application starts up."""
        deadline = time.time() + wait_s
        while time.time() < deadline:
            self._send([UPD_ERASE])
            r = self._answer(UPD_ERASE)
            if r and r[1]:
                return True
            time.sleep(0.5)
        return False

    def baud(self, rate):
        self._send([UPD_CTRL, CTRL_BAUD] + list(rate.to_bytes(4, 'little')))
        self._answer(UPD_CTRL)

    def write(self, data):
        for i in range(0, len(data), 30):
            chunk = data[i:i + 30]
            self._send([UPD_WRITE, len(chunk)] + list(chunk))
            r = self._answer(UPD_WRITE)
            if not r or r[1] != len(chunk):
                raise RuntimeError('keyboard left update mode')

    def poll(self, timeout):
        r = self._read(int(timeout * 1000))
        if r and r[0] == UPD_READ:
            self.rx += bytes(r[2:2 + r[1]])

    def get_ack(self):
        """One SLIP frame, or None after ACK_TIMEOUT_S."""
        deadline = time.time() + ACK_TIMEOUT_S
        while True:
            first = self.rx.find(0xC0)
            second = self.rx.find(0xC0, first + 1) if first >= 0 else -1
            if second > first + 1:
                frame = self.rx[first + 1:second]
                del self.rx[:second + 1]
                return frame
            if second == first + 1:   # two delimiters back to back: keep the second as a start
                del self.rx[:second]
                continue
            left = deadline - time.time()
            if left <= 0:
                return None
            self.poll(min(left, 0.05))


def crc8(data):
    crc = 0
    for b in data:
        crc ^= b
        for _ in range(8):
            crc = ((crc << 1) ^ 0x07) & 0xFF if crc & 0x80 else (crc << 1) & 0xFF
    return crc


def link_enter_dfu(t, baud):
    """Sends BT_CMD_ENTER_DFU as a link frame at the application's baud rate (an application whose
    link speed differs from the keyboard's current STM32 firmware)."""
    t.baud(baud)
    body = [MSG_BT_CMD, 1, BT_CMD_ENTER_DFU]
    t.write(bytes([0]))                 # wake byte: starts the nRF's receiver
    time.sleep(0.01)
    t.write(bytes([LINK_SOF] + body + [crc8(body)]))
    time.sleep(0.05)
    t.baud(BOOTLOADER_BAUD)


def try_start(t, pkt):
    t.rx.clear()                        # whatever the application printed
    t.write(pkt)
    return t.get_ack() is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('zip', nargs='?', help='HY0020 DFU package (omit with --erase-pairings to erase only)')
    ap.add_argument('--erase-pairings', action='store_true',
                    help="erase every BLE pairing (format the HY0020's file system), after the update if any")
    ap.add_argument('--enter', help='bytes sent to the HY0020 application first to make it enter its bootloader')
    ap.add_argument('--link-baud', type=int, help="send the link's enter-DFU command at this baud rate first")
    args = ap.parse_args()
    if not args.zip and not args.erase_pairings:
        ap.error('a zip, --erase-pairings, or both')
    if not args.zip:
        erase(Tunnel())
        return
    with zipfile.ZipFile(args.zip) as z:
        app = json.loads(z.read('manifest.json'))['manifest']['application']
        bin_, dat = z.read(app['bin_file']), z.read(app['dat_file'])

    t = Tunnel()
    if not t.ctrl(CTRL_STATE):
        print('Press BT_UPDT on the keyboard (update mode)...')
        while not t.ctrl(CTRL_STATE):
            time.sleep(0.5)
    steps = dfu_steps(bin_, dat)
    kind, pkt, wait, done = steps[0]
    if args.link_baud:
        link_enter_dfu(t, args.link_baud)
    if args.enter:
        t.write(args.enter.encode())
    time.sleep(1.0)                     # the application restarts in the bootloader
    # Normally the bootloader is waiting now (it stays in serial DFU when the application sent it
    # there). Else: a reset pulse and START while the bootloader listens after it; this needs
    # P0.21 to work as the reset pin.
    ok = try_start(t, pkt)
    for _ in range(ENTER_ATTEMPTS if not ok else 0):
        t.ctrl(CTRL_DFU)
        time.sleep(RESET_TO_START_S)
        if try_start(t, pkt):
            ok = True
            break
    if not ok:
        t.ctrl(CTRL_EXIT)
        sys.exit('no answer from the HY0020 bootloader')
    time.sleep(wait)

    for kind, pkt, wait, done in steps[1:]:
        ack = None
        for _ in range(SEND_ATTEMPTS):
            t.rx.clear()                # a late ACK of an earlier try must not count for this one
            t.write(pkt)
            ack = t.get_ack()
            if ack is not None:
                break
        if ack is None:
            t.ctrl(CTRL_EXIT)
            sys.exit(f'no ACK ({kind})')
        if wait:
            time.sleep(wait)
        if kind == 'data':
            print(f'\r{done * 100 // len(bin_):3d}%', end='', flush=True)
    print('\ractivating...')
    time.sleep(FLASH_PAGE_ERASE_S + FLASH_PAGE_WRITE_S + 0.5)
    t.ctrl(CTRL_EXIT)
    print('done')
    if args.erase_pairings:
        time.sleep(2.0)                 # the application starts again
        erase(t)


def erase(t):
    if t.ctrl(CTRL_STATE):
        t.ctrl(CTRL_EXIT)               # update mode holds the link: give it back first
        time.sleep(2.0)
    if not t.erase_pairings():
        sys.exit('the keyboard did not take the command (update mode still on?)')
    print('pairings erased: the HY0020 restarts; remove the keyboard on your devices and pair again')


if __name__ == '__main__':
    main()
