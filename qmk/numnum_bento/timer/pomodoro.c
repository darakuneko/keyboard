/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "pomodoro.h"
#include "../config/trackpad_config.h"
#include "drivers/haptic/drv2605l.h"

// Internal timer variables
static uint32_t last_update = 0;
static uint16_t total_seconds = 0;

// Break notification variables
static uint32_t last_break_vibration = 0;       // Last break vibration timestamp
static uint16_t break_vibration_interval = 2000; // Break vibration interval in ms
static uint32_t break_notification_start_time = 0; // Start time of break notification
static uint16_t break_auto_dismiss_timeout = 30000; // Auto dismiss break notification after 30 seconds
static uint16_t break_touch_tap_term = 1000;    // Time window for double tap detection (ms)

// Double tap detection variables
static uint32_t last_tap_time = 0;             // Last tap timestamp
static bool first_tap_detected = false;        // First tap detected flag
static uint8_t last_finger_count = 0;          // Last detected finger count
static uint8_t hf_shourt_waveform = 53;        // Short vibration pattern

// Start Pomodoro timer
void pomodoro_start(void) {

    // Do nothing if already active
    if (timer_active) {
        return;
    }

    // Set state to WORK
    phase = POMODORO_WORK;
    current_work_interval = 0;
    current_pomodoro_cycle = 0;
    timer_active = true;
    
    // Set time
    total_seconds = pomodoro_config.work_time * 60;
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    
    // Start notification - haptic pattern for work start
    if (pomodoro_config.notify_haptic_enable) {
        drv2605l_pulse(pomodoro_config.work_hf_pattern);
    }
    
    // Initialize timer update time
    last_update = timer_read32();
}

// Cancel Pomodoro timer
void pomodoro_cancel(void) {
    if (!timer_active) {
        return;
    }
    
    phase = POMODORO_IDLE;
    timer_active = false;
    minutes = 0;
    seconds = 0;
    current_work_interval = 0;
    current_pomodoro_cycle = 0;
    total_seconds = 0;
    
    // Cancel notification - using generic haptic pattern
    if (pomodoro_config.notify_haptic_enable) {
        drv2605l_pulse(hf_shourt_waveform);
    }
}

// Toggle Pomodoro timer
void pomodoro_toggle(void) {
    if (pomodoro_config.notify_haptic_enable) {
        drv2605l_pulse(hf_shourt_waveform);
    }
    
    if (timer_active) {
        pomodoro_cancel();
    } else {
        pomodoro_start();
    }
    send_pomodoro_config(&pomodoro_config);
}

// Advance Pomodoro state to next work_interval
static void pomodoro_next_work_interval(void) {
    switch (phase) {
        case POMODORO_WORK:
            current_work_interval++;
            
            if (current_work_interval >= pomodoro_config.work_interval) {
                phase = POMODORO_LONG_BREAK;
                total_seconds = pomodoro_config.long_break_time * 60;
                current_work_interval = 0;
                // Increment count before entering LONG_BREAK
                current_pomodoro_cycle++;
            } else {
                phase = POMODORO_BREAK;
                total_seconds = pomodoro_config.break_time * 60;
            }
            break;
            
        case POMODORO_BREAK:
            phase = POMODORO_WORK;
            total_seconds = pomodoro_config.work_time * 60;
            break;
            
        case POMODORO_LONG_BREAK:
            // Check LONG_BREAK work_interval limit
            if (pomodoro_config.pomodoro_cycle > 0 && 
                current_pomodoro_cycle >= pomodoro_config.pomodoro_cycle) {
                // When reach configured count
                if (pomodoro_config.continuous_mode) {
                    // With continuous mode: restart from work work_interval
                    phase = POMODORO_WORK;
                    total_seconds = pomodoro_config.work_time * 60;
                    // Keep timer active
                    timer_active = true;
                    // Reset counter
                    current_pomodoro_cycle = 0;
                } else {
                    // Without continuous mode: stop timer
                    phase = POMODORO_IDLE;
                    timer_active = false;
                    total_seconds = 0;
                    // Reset counter
                    current_pomodoro_cycle = 0;
                }
            } else {
                // Behavior after long break based on continuous_mode setting
                if (pomodoro_config.continuous_mode) {
                    // Auto-restart: start next work session
                    phase = POMODORO_WORK;
                    total_seconds = pomodoro_config.work_time * 60;
                    // Keep timer active
                    timer_active = true;
                } else {
                    // No auto-restart: stop timer
                    phase = POMODORO_IDLE;
                    timer_active = false;
                    total_seconds = 0;
                }
            }
            break;
            
        default:
            break;
    }
    
    minutes = total_seconds / 60;
    seconds = total_seconds % 60;
    
    // State change notification
    pomodoro_notify();
    
    // Send configuration as state has changed
    send_pomodoro_active_status(&pomodoro_config);
}

// Update Pomodoro timer
void pomodoro_update(void) {
    if (!timer_active) {
        return;
    }
    
    uint32_t current_time = timer_read32();
    uint32_t elapsed = timer_elapsed32(last_update);
    
    // Update every 1 second
    if (elapsed >= 1000) {
        last_update = current_time;
        
        if (total_seconds > 0) {
            total_seconds--;
            minutes = total_seconds / 60;
            seconds = total_seconds % 60;
            
            // Notify at 1 minute remaining
            if (total_seconds == 60 && pomodoro_config.notify_haptic_enable) {
                // Select haptic pattern based on current state
                if (phase == POMODORO_WORK) {
                    drv2605l_pulse(pomodoro_config.work_hf_pattern);
                } else {
                    drv2605l_pulse(pomodoro_config.break_hf_pattern);
                }
            }
        } else {
            // Timer ended, proceed to next work_interval
            pomodoro_next_work_interval();
        }
    }
    
    // Handle break vibration
    if (waiting_for_break_touch && 
        ((phase == POMODORO_BREAK) || (phase == POMODORO_LONG_BREAK))) {
        
        // Auto dismiss notification after timeout (30 seconds)
        if (timer_elapsed32(break_notification_start_time) >= break_auto_dismiss_timeout) {
            waiting_for_break_touch = false;
            // Optional: can add a short feedback to indicate auto-dismiss
            return;
        }
        
        // Check if it's time for the next vibration and haptic feedback is enabled
        if (timer_elapsed32(last_break_vibration) >= break_vibration_interval && pomodoro_config.notify_haptic_enable) {
            // Vibrate again using break pattern
            drv2605l_pulse(pomodoro_config.break_hf_pattern);
            
            // Long break: double pulse to make it more distinguishable
            if (phase == POMODORO_LONG_BREAK) {
                drv2605l_pulse(pomodoro_config.break_hf_pattern);
            }
            
            last_break_vibration = timer_read32();
        }
    }
}

// Pomodoro state notification
void pomodoro_notify(void) {
    if (!pomodoro_config.notify_haptic_enable) {
        switch (phase) {
            case POMODORO_BREAK:
            case POMODORO_LONG_BREAK:
                waiting_for_break_touch = false;
                last_break_vibration = timer_read32();
                break;
                
            default:
                waiting_for_break_touch = false;
                break;
        }
        return;
    }

    // Haptic feedback based on current state
    switch (phase) {
        case POMODORO_WORK:
            // Work start: wake haptic pattern
            drv2605l_pulse(pomodoro_config.work_hf_pattern);
            waiting_for_break_touch = false;
            break;
            
        case POMODORO_BREAK:
            // Break start: break haptic pattern
            drv2605l_pulse(pomodoro_config.break_hf_pattern);
            waiting_for_break_touch = true;
            last_break_vibration = timer_read32();
            break_notification_start_time = timer_read32(); // Set start time for auto dismiss
            break;
            
        case POMODORO_LONG_BREAK:
            // Long break: same pattern as normal break but with double pulse
            drv2605l_pulse(pomodoro_config.break_hf_pattern);
            // Vibrate twice to make it more distinguishable
            drv2605l_pulse(pomodoro_config.break_hf_pattern);
            waiting_for_break_touch = true;
            last_break_vibration = timer_read32();
            break_notification_start_time = timer_read32(); // Set start time for auto dismiss
            break;
            
        default:
            waiting_for_break_touch = false;
            break;
    }
}

// Process touch event for Pomodoro break states
void pomodoro_process_touch(uint8_t finger_cnt) {
    uint32_t current_time = timer_read32();
    
    // Debug current status
    
    // If we're waiting for a touch during a break, handle double tap
    if (waiting_for_break_touch && 
        ((phase == POMODORO_BREAK) || (phase == POMODORO_LONG_BREAK))) {

        // Detect finger down (1) after it was up (0)
        bool finger_down_event = (finger_cnt == 1) && (last_finger_count == 0);
        if (finger_down_event) {
            if (first_tap_detected && timer_elapsed32(last_tap_time) <= break_touch_tap_term) {
                // Double tap detected, stop the continuous vibration
                waiting_for_break_touch = false;
                first_tap_detected = false;
                last_finger_count = 0;
                // Acknowledge with a short vibration
                if (pomodoro_config.notify_haptic_enable) {
                    drv2605l_pulse(hf_shourt_waveform);
                }
            } else {
                // First tap or too much time elapsed, treat as first tap
                first_tap_detected = true;
                last_tap_time = current_time;
            }
        }
    } else {
        // Reset tap state when not in break mode
        first_tap_detected = false;
    }
    last_finger_count = finger_cnt;
}
