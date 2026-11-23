# Trident40 Wireless: STM32L432KB (128 KB flash; the default L432 linker script is the 256 KB KC)
MCU_LDSCRIPT = STM32L432xB

# BLE through the HY0020 (nRF52832) on USART1, see ble_link.c
BLUETOOTH_ENABLE = yes
BLUETOOTH_DRIVER = custom

SRC += ble_link.c power.c battery.c hy0020_update.c
