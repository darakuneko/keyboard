# Build Options
#   change yes to no to disable
#
# MCU name
MCU = STM32F401

# Bootloader selection
BOOTLOADER = stm32-dfu

BOOTMAGIC_ENABLE = no      # Enable Bootmagic Lite
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no       # Enable keyboard RGB underglow
ENCODER_ENABLE = yes        # Enable rotary encoder support
AUDIO_ENABLE = no           # Audio output
RGB_MATRIX_ENABLE = yes # Do not enable with RGBLIGHT
RGB_MATRIX_DRIVER = WS2812
WS2812_DRIVER = pwm

OLED_ENABLE = yes
OLED_DRIVER = SSD1306
VIA_ENABLE = yes
SRC += qmk_rc.c
DYNAMIC_TAPPING_TERM_ENABLE = yes
WPM_ENABLE = yes
