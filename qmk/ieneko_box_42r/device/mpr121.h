/*
 * MPR121 capacitive touch sensor driver for QMK
 */

#pragma once

#include "quantum.h"
#include "qmk_settings.h"

// MPR121 I2C addresses (both 0x5A and 0x5B supported)
#define MPR121_I2C_ADDRESS_1 (0x5A << 1)
#define MPR121_I2C_ADDRESS_2 (0x5B << 1)

// MPR121 register definitions
#define MPR121_TOUCH_STATUS_L       0x00

// Touch/release threshold registers (base addresses)
#define MPR121_TOUCH_THRESHOLD_0    0x41
#define MPR121_RELEASE_THRESHOLD_0  0x42

// Configuration registers
#define MPR121_DEBOUNCE             0x5B
#define MPR121_CONFIG1              0x5C
#define MPR121_CONFIG2              0x5D
#define MPR121_ELECTRODE_CONFIG     0x5E

// Soft reset command
#define MPR121_SOFT_RESET           0x80
#define MPR121_RESET_VALUE          0x63

// Number of electrodes used
#define MPR121_ELECTRODE_COUNT      8

// Default threshold values (lower = more sensitive)
#define MPR121_DEFAULT_TOUCH_THRESHOLD    6
#define MPR121_DEFAULT_RELEASE_THRESHOLD  3

// Function prototypes
bool mpr121_init(void);
void mpr121_scan(void);
void mpr121_debug_status(void);

// Data structure
typedef struct {
    bool          ele0_touched;
    bool          ele1_touched;
    bool          ele2_touched;
    bool          ele3_touched;
    bool          ele4_touched;
    bool          ele5_touched;
    bool          ele6_touched;
    bool          ele7_touched;
    bool          slider_up;
    bool          slider_down;
} mpr121_data_t;

// Global variables (0x5A)
extern uint16_t mpr121_touch_state_1;
extern uint16_t mpr121_last_touch_state_1;
extern mpr121_data_t mpr121_data_1;

// Global variables (0x5B)
extern uint16_t mpr121_touch_state_2;
extern uint16_t mpr121_last_touch_state_2;
extern mpr121_data_t mpr121_data_2;