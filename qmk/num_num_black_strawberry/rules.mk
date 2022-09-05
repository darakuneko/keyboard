# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

LTO_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
SRC += iqs5xx.c i2c_master.c qmk_rc.c