/*
 * MPR121 capacitive touch sensor driver implementation
 */

#include "mpr121.h"
#include "i2c_master.h"
#include <hal.h>


// Global touch state variables (0x5A)
uint16_t mpr121_touch_state_1 = 0;
uint16_t mpr121_last_touch_state_1 = 0;
mpr121_data_t mpr121_data_1 = {0};

// Global touch state variables (0x5B)
uint16_t mpr121_touch_state_2 = 0;
uint16_t mpr121_last_touch_state_2 = 0;
mpr121_data_t mpr121_data_2 = {0};

// Detected I2C addresses
static bool mpr121_1_present = false;  // 0x5A
static bool mpr121_2_present = false;  // 0x5B

// Error tracking for auto-recovery
static uint16_t mpr121_1_error_count = 0;
static uint16_t mpr121_2_error_count = 0;
static uint32_t mpr121_1_last_error_time = 0;
static uint32_t mpr121_2_last_error_time = 0;
#define MPR121_MAX_CONSECUTIVE_ERRORS 10  // Increased from 5
#define MPR121_ERROR_RESET_INTERVAL_MS 2000  // Increased from 1000ms
#define MPR121_OUT_OF_RANGE 0x04

// Forward declarations
static void mpr121_debug_electrode(uint8_t address, uint8_t electrode, const char *label);

// Touch state machine for tap vs slider detection
typedef enum {
    STATE_IDLE,
    STATE_TOUCHED,      // Electrode is being touched
    STATE_WAITING       // Released, waiting to determine tap or slider
} touch_state_t;

// State machine for 0x5A
static touch_state_t state_1 = STATE_IDLE;
static uint8_t candidate_electrode_1 = 0xFF;
static uint16_t release_time_1 = 0;
static bool slider_executed_1 = false;
static uint16_t touch_start_time_1 = 0;
static bool long_press_active_1 = false;
static uint16_t last_long_press_time_1 = 0;
static uint16_t last_slider_time_1 = 0;
static uint16_t last_tap_time_1 = 0;

// State machine for 0x5B
static touch_state_t state_2 = STATE_IDLE;
static uint8_t candidate_electrode_2 = 0xFF;
static uint16_t release_time_2 = 0;
static bool slider_executed_2 = false;
static uint16_t touch_start_time_2 = 0;
static bool long_press_active_2 = false;
static uint16_t last_long_press_time_2 = 0;
static uint16_t last_slider_time_2 = 0;
static uint16_t last_tap_time_2 = 0;

// I2C retry interval
#define MPR121_I2C_RETRY_INTERVAL_MS 10

// Long press threshold (first activation)
#define MPR121_LONG_PRESS_MS 1000

// Slider repeat interval
#define MPR121_SLIDER_INTERVAL_MS 80

/**
 * Write a single register to MPR121
 */
static bool mpr121_write_register(uint8_t address, uint8_t reg, uint8_t value) {
    i2c_status_t status = i2c_write_register(address, reg, &value, 1, MPR121_I2C_RETRY_INTERVAL_MS);    
    if (status != I2C_STATUS_SUCCESS) {
        uprintf("MPR121 [0x%02X] write failed: reg=0x%02X, status=%d, time=%lu\n",
                address, reg, status, timer_read32());
    }
    return (status == I2C_STATUS_SUCCESS);
}

#ifndef I2C_DRIVER
    #define I2C_DRIVER I2CD1  // I2C0の場合
    // または
    // #define I2C_DRIVER I2CD2  // I2C1の場合（halconf.h必須）
#endif

void i2c_rp2040_hardware_reset(void) {
    uprintf("I2C: Starting hardware reset\n");
        wait_ms(10000);
    // 1. I2Cペリフェラルを停止
    i2cStop(&I2C_DRIVER);
    wait_ms(500);
    
    // 2. GPIOを一時的に入力モードに
    palSetLineMode(I2C1_SDA_PIN, PAL_MODE_INPUT);
    palSetLineMode(I2C1_SCL_PIN, PAL_MODE_INPUT);
    wait_ms(100);
    
    // 3. I2C機能に戻す
    palSetLineMode(I2C1_SDA_PIN, PAL_MODE_ALTERNATE(3) | PAL_RP_PAD_SLEWFAST | PAL_RP_PAD_DRIVE12 | PAL_RP_GPIO_OE);
    palSetLineMode(I2C1_SCL_PIN, PAL_MODE_ALTERNATE(3) | PAL_RP_PAD_SLEWFAST | PAL_RP_PAD_DRIVE12 | PAL_RP_GPIO_OE);
    
    // 4. I2Cペリフェラルを再初期化
    wait_ms(20);
    mpr121_init();
    wait_ms(20);
    
    uprintf("I2C: Hardware reset complete\n");
}

/**
 * Initialize a single MPR121 sensor (stabilized version)
 * Tuned for 3.3V VDD and small/medium electrode size
 */
static bool mpr121_init_device(uint8_t address) {
    // --- Soft reset ---
    mpr121_write_register(address, MPR121_SOFT_RESET, MPR121_RESET_VALUE);

    // --- Stop mode during configuration ---
    mpr121_write_register(address, MPR121_ELECTRODE_CONFIG, 0x00);

    // --- Touch / Release thresholds ---
    // Typical range: Touch=6〜12, Release=3〜6
    for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
        mpr121_write_register(address, MPR121_TOUCH_THRESHOLD_0 + (i * 2), 10);
        mpr121_write_register(address, MPR121_RELEASE_THRESHOLD_0 + (i * 2), 6);
    }

    // --- Debounce configuration ---
    // DR=1, DT=1 → minimal debounce
    mpr121_write_register(address, MPR121_DEBOUNCE, 0x11);

    // --- AFE and filter configuration ---
    // CONFIG1: CDC=8µA（reduced from 32µA for stable small electrodes）
    mpr121_write_register(address, MPR121_CONFIG1, 0x08);

    // CONFIG2: CDT=0.5µs, SFI=0(4 samples), ESI=16ms
    mpr121_write_register(address, MPR121_CONFIG2, 0x41);

    // --- Baseline filter settings ---
    // Balanced between responsiveness and noise suppression
    mpr121_write_register(address, 0x2B, 0x01);  // MHD Rising
    mpr121_write_register(address, 0x2C, 0x01);  // NHD Rising
    mpr121_write_register(address, 0x2D, 0x00);  // NCL Rising
    mpr121_write_register(address, 0x2E, 0x00);  // FDL Rising
    mpr121_write_register(address, 0x2F, 0x01);  // MHD Falling
    mpr121_write_register(address, 0x30, 0x01);  // NHD Falling
    mpr121_write_register(address, 0x31, 0xFF);  // NCL Falling
    mpr121_write_register(address, 0x32, 0x02);  // FDL Falling
    mpr121_write_register(address, 0x33, 0x00);  // NHD Touched
    mpr121_write_register(address, 0x34, 0x00);  // NCL Touched
    mpr121_write_register(address, 0x35, 0x00);  // FDL Touched

    // --- Auto-configuration settings ---
    // Adjusted for 3.3V operation and small electrode capacitance
    mpr121_write_register(address, 0x7D, 220);   // USL (Upper Side Limit)
    mpr121_write_register(address, 0x7E, 10);    // LSL (Lower Side Limit)
    mpr121_write_register(address, 0x7F, 200);   // TL (Target Level)

    // Auto-config Control 0:
    // FFI=0, RETRY=2 (3x), BVA=0, ARE=1, ACE=1
    mpr121_write_register(address, 0x7B, 0x0F);

    // Auto-config Control 1:
    // No interrupt enables
    mpr121_write_register(address, 0x7C, 0x00);

    // --- Enable electrodes with baseline initialization ---
    // CL=10 (charge level), ELE_EN=8 electrodes
    mpr121_write_register(address, MPR121_ELECTRODE_CONFIG, 0x88);

    return true;
}

/**
 * Initialize MPR121 sensors
 */
bool mpr121_init(void) {
    i2c_init();
    uprintf("MPR121: Initializing I2C devices...\n");
    
    // Test I2C connectivity - check both addresses
    i2c_status_t ping_status_1 = i2c_ping_address(MPR121_I2C_ADDRESS_1, 100);
    i2c_status_t ping_status_2 = i2c_ping_address(MPR121_I2C_ADDRESS_2, 100);

    uprintf("MPR121: Ping 0x%02X -> status=%d\n", MPR121_I2C_ADDRESS_1, ping_status_1);
    uprintf("MPR121: Ping 0x%02X -> status=%d\n", MPR121_I2C_ADDRESS_2, ping_status_2);

    if (ping_status_1 == I2C_STATUS_SUCCESS) {
        mpr121_1_present = true;
        uprintf("MPR121#1 [0x%02X]: Detected, initializing...\n", MPR121_I2C_ADDRESS_1);
        mpr121_init_device(MPR121_I2C_ADDRESS_1);
        uprintf("MPR121#1 [0x%02X]: Initialization complete\n", MPR121_I2C_ADDRESS_1);
    }

    if (ping_status_2 == I2C_STATUS_SUCCESS) {
        mpr121_2_present = true;
        uprintf("MPR121#2 [0x%02X]: Detected, initializing...\n", MPR121_I2C_ADDRESS_2);
        mpr121_init_device(MPR121_I2C_ADDRESS_2);
        uprintf("MPR121#2 [0x%02X]: Initialization complete\n", MPR121_I2C_ADDRESS_2);
    }

    if (!mpr121_1_present && !mpr121_2_present) {
        uprintf("MPR121: No devices found!\n");
        return false;
    }

    uprintf("MPR121: Init OK (device1=%d, device2=%d)\n", mpr121_1_present, mpr121_2_present);
    return true;
}

/**
 * Check device status and detect errors
 */
static bool mpr121_check_device_status(uint8_t address, uint16_t *error_count, uint32_t *last_error_time, const char *label) {
    uint8_t status_data;

    // Read Out-of-Range status register (0x02)
    i2c_status_t status = i2c_read_register(address, 0x02, &status_data, 1, MPR121_I2C_RETRY_INTERVAL_MS);

    if (status != I2C_STATUS_SUCCESS) {
        // I2C errors are logged but ignored - continue touch detection
        static uint32_t last_i2c_log_time = 0;
        uint32_t current_time = timer_read32();

        // Log periodically to avoid spam
        if (timer_elapsed32(last_i2c_log_time) > 2000) {
            uprintf("%s [0x%02X]: I2C read warning - reg=0x02, status=%d (ignored)\n",
                    label, address, status);
            last_i2c_log_time = current_time;
        }

        // Reset error count and continue
        *error_count = 0;
        return true;  // Continue touch detection despite I2C error
    }

    // Check for out-of-range condition (bit 2)
    if (status_data & MPR121_OUT_OF_RANGE) {
        // OOR errors are logged but ignored - I2C communication is stable
        // Touch detection continues normally despite OOR conditions

        // Only log periodically to avoid spam (every ~2 seconds)
        static uint32_t last_oor_log_time = 0;
        uint32_t current_time = timer_read32();

        if (timer_elapsed32(last_oor_log_time) > 2000) {
            // Read detailed OOR status for each electrode
            uint8_t oor_l, oor_h;
            i2c_read_register(address, 0x02, &oor_l, 1, MPR121_I2C_RETRY_INTERVAL_MS);
            i2c_read_register(address, 0x03, &oor_h, 1, MPR121_I2C_RETRY_INTERVAL_MS);
            uint16_t oor_electrodes = oor_l | (oor_h << 8);

            uprintf("%s [0x%02X]: OOR warning - electrodes=0x%04X [",
                    label, address, oor_electrodes);
            for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
                if (oor_electrodes & (1 << i)) {
                    uprintf("%d ", i);
                }
            }
            uprintf("] (ignored, touch detection continues)\n");

            last_oor_log_time = current_time;
        }

        // Reset error count and continue normal operation
        *error_count = 0;
        // Return true to allow touch detection to continue
    }

    // Reset error count on successful read
    *error_count = 0;
    return true;
}

/**
 * Scan a single MPR121 device
 */
static void mpr121_scan_device(uint8_t address, uint16_t *touch_state, uint16_t *last_touch_state,
                                mpr121_data_t *data, touch_state_t *state_ptr,
                                uint8_t *candidate_ptr, uint16_t *release_time_ptr, bool *slider_executed_ptr,
                                uint16_t *touch_start_time_ptr, bool *long_press_active_ptr,
                                uint16_t *last_long_press_time_ptr, uint16_t *last_slider_time_ptr,
                                uint16_t *last_tap_time_ptr,
                                const char *label) {
    uint8_t i2c_data[2];

    // Clear previous scan's events (one-shot events)
    data->ele0_touched = false;
    data->ele1_touched = false;
    data->ele2_touched = false;
    data->ele3_touched = false;
    data->ele4_touched = false;
    data->ele5_touched = false;
    data->ele6_touched = false;
    data->ele7_touched = false;
    data->slider_up = false;
    data->slider_down = false;

    // Read touch status register
    i2c_status_t status = i2c_read_register(address, MPR121_TOUCH_STATUS_L, i2c_data, 2, MPR121_I2C_RETRY_INTERVAL_MS);
    if (status != I2C_STATUS_SUCCESS) {
        uprintf("%s [0x%02X]: Touch read failed - reg=0x%02X, status=%d, time=%lu\n",
                label, address, MPR121_TOUCH_STATUS_L, status, timer_read32());
        return;
    }

    // Update touch state
    *last_touch_state = *touch_state;
    *touch_state = i2c_data[0] | (i2c_data[1] << 8);

    // State machine logic
    switch (*state_ptr) {
        case STATE_IDLE:
            // Check for any touch (only accept when all electrodes are released)
            if (*last_touch_state == 0) {
                for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
                    uint16_t electrode_mask = (1 << i);
                    bool is_touched = (*touch_state & electrode_mask);

                    if (is_touched) {
                        // New touch detected
                        *candidate_ptr = i;
                        *state_ptr = STATE_TOUCHED;
                        *slider_executed_ptr = false;
                        *touch_start_time_ptr = timer_read();
                        *long_press_active_ptr = false;
                        break;
                    }
                }
            }
            break;

        case STATE_TOUCHED:
            // Check for slider (different electrode touched while holding)
            for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
                if (i == *candidate_ptr) continue;

                uint16_t electrode_mask = (1 << i);
                bool is_touched = (*touch_state & electrode_mask);

                if (is_touched) {
                    // Different electrode touched while holding: Slider
                    // Only emit event if both slider interval and tap interval have elapsed
                    if (timer_elapsed(*last_slider_time_ptr) >= MPR121_SLIDER_INTERVAL_MS &&
                        timer_elapsed(*last_tap_time_ptr) >= MPR121_SLIDER_INTERVAL_MS) {
                        if (*long_press_active_ptr) {
                            *long_press_active_ptr = false;
                        }
                        if (*candidate_ptr > i) {
                            data->slider_up = true;
                        } else {
                            data->slider_down = true;
                        }
                        *last_slider_time_ptr = timer_read();
                    }
                    // Update candidate and stay in TOUCHED state for continuous sliding
                    *candidate_ptr = i;
                    *touch_start_time_ptr = timer_read();
                    *slider_executed_ptr = true;
                    return;
                }
            }

            // Check if candidate electrode is released or held long enough
            if (*candidate_ptr != 0xFF) {
                uint16_t electrode_mask = (1 << *candidate_ptr);
                bool was_touched = (*last_touch_state & electrode_mask);
                bool is_touched = (*touch_state & electrode_mask);

                if (was_touched && !is_touched) {
                    // Candidate electrode released
                    if (*long_press_active_ptr) {
                        *long_press_active_ptr = false;
                    }
                    *release_time_ptr = timer_read();
                    *state_ptr = STATE_WAITING;
                } else if (is_touched && timer_elapsed(*touch_start_time_ptr) >= MPR121_LONG_PRESS_MS) {
                    // Long press active - emit press event at tapping_term intervals
                    if (!*long_press_active_ptr) {
                        *long_press_active_ptr = true;
                        *last_long_press_time_ptr = timer_read();
                        // Emit first event
                        switch (*candidate_ptr) {
                            case 0: data->ele0_touched = true; break;
                            case 1: data->ele1_touched = true; break;
                            case 2: data->ele2_touched = true; break;
                            case 3: data->ele3_touched = true; break;
                            case 4: data->ele4_touched = true; break;
                            case 5: data->ele5_touched = true; break;
                            case 6: data->ele6_touched = true; break;
                            case 7: data->ele7_touched = true; break;
                        }
                        *last_tap_time_ptr = timer_read();  // Update tap timestamp
                    } else if (timer_elapsed(*last_long_press_time_ptr) >= QS.tapping_term) {
                        // Emit event at tapping_term intervals
                        *last_long_press_time_ptr = timer_read();
                        switch (*candidate_ptr) {
                            case 0: data->ele0_touched = true; break;
                            case 1: data->ele1_touched = true; break;
                            case 2: data->ele2_touched = true; break;
                            case 3: data->ele3_touched = true; break;
                            case 4: data->ele4_touched = true; break;
                            case 5: data->ele5_touched = true; break;
                            case 6: data->ele6_touched = true; break;
                            case 7: data->ele7_touched = true; break;
                        }
                        *last_tap_time_ptr = timer_read();  // Update tap timestamp
                    }
                }
            }
            break;

        case STATE_WAITING:
            // Check if all electrodes released
            if (*touch_state == 0) {
                // All released: emit tap event only if no slider was executed
                if (!*slider_executed_ptr) {
                    switch (*candidate_ptr) {
                        case 0: data->ele0_touched = true; break;
                        case 1: data->ele1_touched = true; break;
                        case 2: data->ele2_touched = true; break;
                        case 3: data->ele3_touched = true; break;
                        case 4: data->ele4_touched = true; break;
                        case 5: data->ele5_touched = true; break;
                        case 6: data->ele6_touched = true; break;
                        case 7: data->ele7_touched = true; break;
                    }
                    *last_tap_time_ptr = timer_read();  // Update tap timestamp
                }
                *candidate_ptr = 0xFF;
                *state_ptr = STATE_IDLE;
                *slider_executed_ptr = false;
            }
            break;
    }
}

/**
 * Scan MPR121 touch status and handle key events
 * Scans both devices with a small delay between them to reduce I2C bus conflicts
 */
void mpr121_scan(void) {
    if (mpr121_1_present) {
        // Check device status before scanning
        if (mpr121_check_device_status(MPR121_I2C_ADDRESS_1, &mpr121_1_error_count, &mpr121_1_last_error_time, "MPR121#1")) {
            mpr121_scan_device(MPR121_I2C_ADDRESS_1, &mpr121_touch_state_1, &mpr121_last_touch_state_1,
                               &mpr121_data_1, &state_1, &candidate_electrode_1, &release_time_1,
                               &slider_executed_1, &touch_start_time_1, &long_press_active_1,
                               &last_long_press_time_1, &last_slider_time_1, &last_tap_time_1, "MPR121#1");
        }
    }

    // Add small delay between device scans when both are present to reduce I2C bus conflicts
    if (mpr121_1_present && mpr121_2_present) {
        wait_us(100);  // 100μs delay between devices
    }

    if (mpr121_2_present) {
        // Check device status before scanning
        if (mpr121_check_device_status(MPR121_I2C_ADDRESS_2, &mpr121_2_error_count, &mpr121_2_last_error_time, "MPR121#2")) {
            mpr121_scan_device(MPR121_I2C_ADDRESS_2, &mpr121_touch_state_2, &mpr121_last_touch_state_2,
                               &mpr121_data_2, &state_2, &candidate_electrode_2, &release_time_2,
                               &slider_executed_2, &touch_start_time_2, &long_press_active_2,
                               &last_long_press_time_2, &last_slider_time_2, &last_tap_time_2, "MPR121#2");
        }
    }
}

/**
 * Debug function to print detailed electrode data
 */
static void mpr121_debug_electrode(uint8_t address, uint8_t electrode, const char *label) {
    uint8_t data[2];

    // Read filtered data (0x04 + electrode*2)
    uint8_t filtered_reg = 0x04 + (electrode * 2);
    i2c_status_t status = i2c_read_register(address, filtered_reg, data, 2, MPR121_I2C_RETRY_INTERVAL_MS);
    uint16_t filtered = (status == I2C_STATUS_SUCCESS) ? (data[0] | (data[1] << 8)) : 0xFFFF;

    // Read baseline value (0x1E + electrode)
    uint8_t baseline_reg = 0x1E + electrode;
    uint8_t baseline = 0;
    status = i2c_read_register(address, baseline_reg, &baseline, 1, MPR121_I2C_RETRY_INTERVAL_MS);

    // Read touch/release thresholds
    uint8_t touch_thresh = 0, release_thresh = 0;
    i2c_read_register(address, MPR121_TOUCH_THRESHOLD_0 + (electrode * 2), &touch_thresh, 1, MPR121_I2C_RETRY_INTERVAL_MS);
    i2c_read_register(address, MPR121_RELEASE_THRESHOLD_0 + (electrode * 2), &release_thresh, 1, MPR121_I2C_RETRY_INTERVAL_MS);

    uprintf("%s ELE%d: Filtered=%d, Baseline=%d (x2=%d), Touch=%d, Release=%d, Delta=%d\n",
            label, electrode, filtered, baseline, baseline << 2, touch_thresh, release_thresh,
            (int)filtered - (int)(baseline << 2));
}

/**
 * Debug function to print current MPR121 status
 */
void mpr121_debug_status(void) {
    if (mpr121_1_present) {
        uint8_t data[16];
        i2c_status_t status;

        // Read touch status
        status = i2c_read_register(MPR121_I2C_ADDRESS_1, MPR121_TOUCH_STATUS_L, data, 2, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uint16_t touch_state = data[0] | (data[1] << 8);
            uprintf("MPR121#1: Touch=0x%04X, Errors=%d\n", touch_state, mpr121_1_error_count);
        } else {
            uprintf("MPR121#1: I2C read failed, Errors=%d\n", mpr121_1_error_count);
        }

        // Read OOR status
        status = i2c_read_register(MPR121_I2C_ADDRESS_1, 0x02, data, 1, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uprintf("MPR121#1: OOR=0x%02X\n", data[0]);
        }

        // Read electrode config
        status = i2c_read_register(MPR121_I2C_ADDRESS_1, MPR121_ELECTRODE_CONFIG, data, 1, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uprintf("MPR121#1: ECR=0x%02X\n", data[0]);
        }

        // Print detailed electrode data for all electrodes
        uprintf("MPR121#1 Electrode Details:\n");
        for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
            mpr121_debug_electrode(MPR121_I2C_ADDRESS_1, i, "  ");
        }
    }

    if (mpr121_2_present) {
        uint8_t data[16];
        i2c_status_t status;

        // Read touch status
        status = i2c_read_register(MPR121_I2C_ADDRESS_2, MPR121_TOUCH_STATUS_L, data, 2, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uint16_t touch_state = data[0] | (data[1] << 8);
            uprintf("MPR121#2: Touch=0x%04X, Errors=%d\n", touch_state, mpr121_2_error_count);
        } else {
            uprintf("MPR121#2: I2C read failed, Errors=%d\n", mpr121_2_error_count);
        }

        // Read OOR status
        status = i2c_read_register(MPR121_I2C_ADDRESS_2, 0x02, data, 1, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uprintf("MPR121#2: OOR=0x%02X\n", data[0]);
        }

        // Read electrode config
        status = i2c_read_register(MPR121_I2C_ADDRESS_2, MPR121_ELECTRODE_CONFIG, data, 1, MPR121_I2C_RETRY_INTERVAL_MS);
        if (status == I2C_STATUS_SUCCESS) {
            uprintf("MPR121#2: ECR=0x%02X\n", data[0]);
        }

        // Print detailed electrode data for all electrodes
        uprintf("MPR121#2 Electrode Details:\n");
        for (uint8_t i = 0; i < MPR121_ELECTRODE_COUNT; i++) {
            mpr121_debug_electrode(MPR121_I2C_ADDRESS_2, i, "  ");
        }
    }
} 
