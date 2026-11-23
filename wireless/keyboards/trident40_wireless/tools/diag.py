#!/usr/bin/env python3
"""Trident40 Wireless: BLE link state over USB (raw HID 0xB3, see hy0020_update.c)."""
import os
import shutil
import struct
import subprocess
import sys
import time

import hid

STATES = {0: 'idle', 1: 'advertising', 2: 'connected'}
RESET_FLAGS = ['firewall', 'option bytes', 'pin', 'power', 'software', 'IWDG', 'WWDG', 'low power']  # RCC_CSR bits 24..31
USB_STATES = {0: 'uninit', 1: 'stop', 2: 'ready', 3: 'selected', 4: 'active', 5: 'suspended'}

# USB first (interface_number >= 0); the BLE HID device (-1) answers too, through the nRF
devs = sorted((d for d in hid.enumerate(0x7A79, 0x1577) if d['usage_page'] == 0xFF60),
              key=lambda d: (d['interface_number'] < 0) != ('--ble' in sys.argv))
path = devs[0]['path'] if devs else None
if not path:
    sys.exit('Trident40 Wireless not found')
dev = hid.device()
dev.open_path(path)
dev.write(bytes([0, 0xB3]) + bytes(31))  # the answer has the nRF counters of the previous ping
time.sleep(0.3)
while dev.read(32, 50):
    pass
dev.write(bytes([0, 0xB3]) + bytes(31))
deadline = time.time() + 1
while time.time() < deadline:
    r = dev.read(32, 100)
    if r and r[0] == 0xB3:
        state, running, hello, hellos, rx, crc, tx = struct.unpack_from('<BB5sxHHHH', bytes(r), 1)
        nrf_rx, nrf_drop, interval, latency, timeout = struct.unpack_from('<HHHHH', bytes(r), 17)
        mv, vbus, usb = struct.unpack_from('<HBB', bytes(r), 27)
        print(f'BLE state     {STATES.get(state, state)}')
        print(f'link running  {bool(running)}')
        print(f'HELLO         {hellos} received, last: proto {hello[0]} fw {hello[1]}.{hello[2]}.{hello[3]} reset pin {"ok" if hello[4] else "NOT SET"}')
        print(f'frames        rx {rx}  crc errors {crc}  tx {tx}')
        print(f'nRF side      rx {nrf_rx}  dropped {nrf_drop}')
        if interval:
            print(f'connection    interval {interval * 1.25:g} ms  latency {latency}'
                  f' (radio every {interval * 1.25 * (latency + 1):g} ms when idle)  timeout {timeout * 10} ms')
        else:
            print('connection    none')
        print(f'battery       {mv} mV')
        print(f'VBUS {vbus}  USB {USB_STATES.get(usb, usb)}')
        break
else:
    print('no answer (old firmware?)')

dev.write(bytes([0, 0xB4]) + bytes(31))
deadline = time.time() + 1
while time.time() < deadline:
    r = dev.read(32, 100)
    if r and r[0] == 0xB4:
        stops, key, irq, other, held, busy, awake, flags = struct.unpack_from('<7IB', bytes(r), 1)
        names = [n for i, n in enumerate(RESET_FLAGS) if flags >> i & 1]
        print(f'Stop          {stops} wake-ups: key {key}  nRF IRQ {irq}  other {other}  VBUS {stops - key - irq - other}')
        print(f'Stop refused  key held {held}  busy {busy}')
        print(f'awake on batt {awake / 1000:.1f} s')
        print(f'last reset    {" ".join(names) or "none"}')
        break
    if r and r[0] == 0xFF:
        print('no power counters (old firmware)')
        break

# nRF power counters (raw HID 0xB6): the answer holds the previous request's counters, so ask twice.
NRF_RESET = ['pin', 'watchdog', 'soft reset', 'lockup', 'wake from System OFF']
dev.write(bytes([0, 0xB6]) + bytes(31))
time.sleep(0.3)
while dev.read(32, 50):
    pass
dev.write(bytes([0, 0xB6]) + bytes(31))
deadline = time.time() + 1
while time.time() < deadline:
    r = dev.read(32, 100)
    if r and r[0] == 0xB6:
        if r[1] < 30:
            print('nRF counters  none (old HY0020 firmware?)')
            break
        up, wakes, busy, uart_ms, uart_n, lat_ms, conns, params, flags, rr = struct.unpack_from('<6IHHBB', bytes(r), 2)
        pct = lambda ms: f'{ms / 1000:.1f} s ({ms / 10 / up:.2f} %)' if up else f'{ms / 1000:.1f} s'
        print(f'nRF uptime    {up // 86400} d {up % 86400 // 3600} h {up % 3600 // 60} min  (reset: '
              f'{" ".join(n for i, n in enumerate(NRF_RESET) if rr >> i & 1) or "power on"})')
        print(f'nRF loop      {wakes} passes ({wakes / up if up else 0:.2f}/s), busy {pct(busy)}')
        print(f'nRF UART rx   on {pct(uart_ms)}, {uart_n} starts{"  [on now]" if flags & 1 else ""}')
        print(f'nRF latency   off {pct(lat_ms)}{"  [off now]" if flags & 2 else ""}')
        print(f'nRF BLE       {conns} connections, {params} parameter changes')
        print(f'nRF clocks    HFCLK crystal {"RUNNING" if flags & 4 else "off"}, DC/DC {"on" if flags & 8 else "OFF"}')
        break
    if r and r[0] == 0xFF:
        print('no nRF counters (old STM32 firmware)')
        break

# Resting battery value (raw HID 0xB7): the median the host is shown, and the samples behind it.
dev.write(bytes([0, 0xB7]) + bytes(31))
deadline = time.time() + 1
while time.time() < deadline:
    r = dev.read(32, 100)
    if r and r[0] == 0xB7:
        n = min(r[1], 5)
        v = struct.unpack_from(f'<{n + 1}H', bytes(r), 2)
        print(f'battery rest  {v[0]} mV (median of {n}: {" ".join(str(x) for x in v[1:])}, newest last)')
        break
    if r and r[0] == 0xFF:
        print('no resting battery value (old STM32 firmware)')
        break

# nRF stall record (raw HID 0xB8): what its loop() was doing when it stopped for 2 s (the watchdog
# restarts it at 4 s). `diag.py --stall` makes it stall on purpose first, to check the record.
STAGES = ['setup', 'link poll', 'handling a link frame', 'send pending reports', 'LEDs / Vial to the STM32',
          'status', 'connection parameters', 'link idle', 'battery sample', 'System OFF check', 'waiting (idle)',
          'slow frame']
BT_CMDS = {1: 'PAIR', 2: 'ADVERTISE', 3: 'ENTER_DFU', 4: 'SELECT_HOST', 5: 'ERASE_ALL', 6: 'TEST_STALL'}
TASK_STATES = ['running', 'ready', 'blocked', 'suspended', 'deleted']
stall = '--stall' in sys.argv
dev.write(bytes([0, 0xB8, 0xA5 if stall else 0]) + bytes(30))
if stall:
    print('nRF stall     requested: wait for the watchdog...')
    time.sleep(8)
    while dev.read(32, 50):
        pass
    dev.write(bytes([0, 0xB8, 0]) + bytes(30))
deadline = time.time() + 1
while time.time() < deadline:
    r = dev.read(32, 100)
    if r and r[0] == 0xB8:
        if not r[1]:
            print('nRF records   none since the STM32 started (no stall, no slow frame)')
            break
        stage, ftype, state, f_loop, f_rx, arg = r[2:8]
        up, pc, lr, a0, a1, a2 = struct.unpack_from('<6I', bytes(r), 8)
        frame = f'frame type 0x{ftype:02X}' + (f' {BT_CMDS.get(arg, arg)}' if ftype == 0x21 else '')
        when = f'nRF uptime {up // 3600} h {up % 3600 // 60} min {up % 60} s'
        if stage == 11:   # not a stall: a frame that took long (the pc field is its duration)
            print(f'nRF records   {r[1]} since the STM32 started. Last one: slow frame, {pc} ms ({frame}), {when}')
            print(f'  stack free  loop {f_loop} words, link rx {f_rx} (255 = that or more)')
            break
        print(f'nRF records   {r[1]} since the STM32 started. Last one: STALL, {when}')
        print(f'  loop() in   {STAGES[stage] if stage < len(STAGES) else stage}'
              + (f' ({frame})' if stage == 2 else '') + f', task {TASK_STATES[state] if state < 5 else state}')
        print(f'  stack free  loop {f_loop} words, link rx {f_rx} (255 = that or more)')
        print(f'  pc 0x{pc:08X}  lr 0x{lr:08X}  callers ' + ' '.join(f'0x{a:08X}' for a in (a0, a1, a2) if a))
        elf = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', 'build', 'nrf', 'trident40_ble.ino.elf')
        tool = shutil.which('arm-none-eabi-addr2line')
        if tool and os.path.exists(elf):
            out = subprocess.run([tool, '-f', '-C', '-e', elf] + [hex(a & ~1) for a in (pc, lr, a0, a1, a2) if a],
                                 capture_output=True, text=True).stdout.split('\n')
            for fn, loc in zip(out[0::2], out[1::2]):
                print(f'    {fn}  {loc}')
        break
    if r and r[0] == 0xFF:
        print('no nRF stall record (old STM32 firmware)')
        break
