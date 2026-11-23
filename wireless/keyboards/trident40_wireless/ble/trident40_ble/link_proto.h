/*
 * Trident40 Wireless: UART link between the STM32L432 (QMK / Vial) and the HY0020 (nRF52832, BLE).
 * The same file is in both firmwares (QMK keyboard directory and the HY0020 sketch): keep them equal.
 *
 * Frame: SOF, TYPE, LEN, PAYLOAD[LEN], CRC8
 *   SOF  = 0x7E
 *   LEN  = payload length, 0..LINK_MAX_PAYLOAD
 *   CRC8 = polynomial 0x07, init 0x00, over TYPE, LEN and PAYLOAD
 * A receiver drops a frame with a bad CRC or with a gap longer than LINK_BYTE_TIMEOUT_MS inside it
 * and hunts for the next SOF. Nothing is acknowledged. The nRF lost bytes with the Adafruit core's
 * Serial (one byte per UARTE DMA transfer, re-armed in an interrupt the SoftDevice can delay); its
 * own receiver (link.cpp) fixes that, and as a second line the STM32 sends every KEYBOARD, CONSUMER
 * and SYSTEM frame twice
 * and the nRF drops one that repeats the previous frame of the same kind: a lost press or release
 * would mean a missing or stuck key. MOUSE is not repeated (relative motion would add up); Vial
 * retries at its own level.
 *
 * Power:
 *   STM32 -> nRF: the nRF switches its UART receiver off LINK_UART_IDLE_MS after the last byte (the
 *   receiver alone costs ~0.9 mA) and waits for a falling edge on RX. The STM32 therefore sends one
 *   0x00 byte first when it has sent nothing for LINK_WAKE_AFTER_MS (or slept in Stop since), and its
 *   frame LINK_WAKE_MS later. The same edge wakes the nRF from System OFF (it then restarts and the
 *   frame is lost; it sends HELLO when it is up).
 *   nRF -> STM32: the STM32 sleeps in Stop 2 between keys on battery. The nRF pulls IRQ (P0.07 ->
 *   PA1) low, waits LINK_IRQ_LEAD_US, sends, and releases IRQ when the frame is out. The STM32 stays
 *   awake while IRQ is low and LINK_RX_HOLD_MS after it.
 *
 * Multi-byte fields are little-endian.
 */
#ifndef TRIDENT40_LINK_PROTO_H
#define TRIDENT40_LINK_PROTO_H

#define LINK_SOF              0x7E
#define LINK_MAX_PAYLOAD      40
#define LINK_BAUD             250000UL    /* STM32 16 MHz / 64 and nRF 250k: both exact. Not 1 Mbaud: the Adafruit core receives one byte per UARTE DMA transfer (re-armed in its IRQ) and lost the frames at 1 Mbaud (2026-10-30) */
#define LINK_BYTE_TIMEOUT_MS  5
#define LINK_PROTO_VERSION    1

/* 50 ms, was 500 until 2026-11-21: at 500 the receiver (~0.9 mA) never went off while typing, about
 * 1.1 mA at the cell for as long as keys came, and a CR2450 sags under that (it recovered with
 * rest, 2026-11-20). The STM32 sends the wake byte after every Stop anyway, so nothing new is used. */
#define LINK_UART_IDLE_MS     50          /* nRF: receiver off after this long without a byte */
#define LINK_WAKE_AFTER_MS    30          /* STM32: send the wake byte after this long without sending */
#define LINK_WAKE_MS          5           /* STM32: wake byte -> frame (the nRF loop task starts its UART) */
#define LINK_IRQ_LEAD_US      300         /* nRF: IRQ low -> frame */
#define LINK_RX_HOLD_MS       20          /* STM32: stay awake after IRQ went high again */

/* ---- STM32 -> nRF ------------------------------------------------------------------------ */
/* KEYBOARD: u8 mods, u8 reserved, u8 keys[6] (boot keyboard layout) */
#define MSG_KEYBOARD          0x10
/* CONSUMER: u16 usage (0 = released) */
#define MSG_CONSUMER          0x11
/* SYSTEM: u16 usage (0x81 power down, 0x82 sleep, 0x83 wake up; 0 = released) */
#define MSG_SYSTEM            0x12
/* MOUSE: u8 buttons, i8 x, i8 y, i8 wheel, i8 pan */
#define MSG_MOUSE             0x13
/* VIAL_IN: 32 bytes, Vial raw HID input report (keyboard -> host) */
#define MSG_VIAL_IN           0x14
/* BATTERY: u8 percent, u16 millivolts (the resting value, see BAT_SAMPLE). Sent at start-up and
 * when the percentage changes. */
#define MSG_BATTERY           0x20
/* BT_CMD: u8 command (BT_CMD_*) [, u8 argument] */
#define MSG_BT_CMD            0x21
/* PING: the nRF answers with STATUS. Sent by the STM32 after its own start and when it needs the
 * state (it keeps no timer: everything else is event driven). */
#define MSG_PING              0x22
/* STATS_REQ: the nRF answers with STATS (its power counters, for tools/diag.py). */
#define MSG_STATS_REQ         0x23

/* 1 was PAIR (forget the selected host's bonds), 4 was SELECT_HOST (four host identities): removed
 * 2026-11-22, the nRF ignores them. */
#define BT_CMD_ADVERTISE      2           /* advertise if not connected (keys pressed while BLE is down) */
#define BT_CMD_ENTER_DFU      3           /* restart in the bootloader's serial DFU (115200 baud), see hy0020_update.c */
#define BT_CMD_ERASE_ALL      5           /* erase the nRF's file system (every bond), then restart: the way back when it is corrupt */
#define BT_CMD_TEST_STALL     6           /* test only: loop() spins until the watchdog restarts the nRF (checks MSG_FAULT end to end) */

/* ---- nRF -> STM32 ------------------------------------------------------------------------ */
/* STATUS: u8 state (BT_STATE_*), u16 frames received, u16 frames dropped (bad CRC or timeout),
 * then the connection's parameters (0 when not connected): u16 interval (x 1.25 ms), u16 slave
 * latency, u16 supervision timeout (x 10 ms). Sent on every change and as the answer to PING. */
#define MSG_STATUS            0x80
/* LEDS: u8 keyboard LED output report (bit 0 Num Lock, 1 Caps Lock, 2 Scroll Lock, ...) */
#define MSG_LEDS              0x81
/* VIAL_OUT: 32 bytes, Vial raw HID output report (host -> keyboard) */
#define MSG_VIAL_OUT          0x82
/* HELLO: sent once after every nRF start. u8 proto_version, u8 fw_major, u8 fw_minor, u8 fw_patch,
 * u8 reset_pin (1 = P0.21 is the reset pin, UICR PSELRESET) */
#define MSG_HELLO             0x83
/* STATS: counters since the nRF's start (all u32 unless noted): uptime s, loop() passes, ms in the
 * busy loop (reports waiting for BLE), ms with the link receiver on, receiver starts, ms with slave
 * latency off (Vial), u16 connections, u16 connection parameter changes, u8 flags (bit 0 receiver
 * on, 1 latency off, 2 HFCLK crystal running, 3 DC/DC on), u8 reset reason (RESETREAS bits 0-3:
 * pin, watchdog, soft reset, lockup; bit 4: woke from System OFF). 30 bytes. */
#define MSG_STATS             0x84

/* BAT_SAMPLE: no payload. The nRF asks for a resting battery sample: sent every
 * LINK_BAT_SAMPLE_PERIOD_S once the link has been quiet for LINK_BAT_SAMPLE_QUIET_S (no key for
 * that long). The STM32 measures the cell the moment the IRQ wakes it from Stop, before this frame
 * is even received (nothing but the standby current has flowed for the ADC's RC filter time), and
 * keeps the sample when the frame turns out to be this one (battery.c). */
#define MSG_BAT_SAMPLE        0x85
#define LINK_BAT_SAMPLE_QUIET_S   60
#define LINK_BAT_SAMPLE_PERIOD_S  300

/* FAULT: loop() on the nRF has not run for 2 s (the watchdog restarts it at 4 s). 30 bytes:
 * u8 stage (fault.h STAGE_*), u8 link frame type being handled, u8 loop task state (FreeRTOS
 * eTaskState: 0 running, 1 ready, 2 blocked, 3 suspended), u8 x 2 stack words never used (loop
 * task, link rx task; 255 = that or more), u8 first payload byte of that frame (BT_CMD_*), u32 uptime s, u32 pc, u32 lr of the
 * loop task, u32 x 3 return addresses found further up its stack. The STM32 keeps the last one.
 * Stage 11 (slow) is not a stall: a frame took 1 s or more; the pc field is then its duration in ms. */
#define MSG_FAULT             0x86

#define BT_STATE_IDLE         0           /* not connected, not advertising */
#define BT_STATE_ADVERTISING  1
#define BT_STATE_CONNECTED    2           /* connected and encrypted (bonded host) */

#endif
