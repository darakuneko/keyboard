POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = custom
SRC += i2c_master.c device/iqs5xx.c config/trackpad_config.c timer/pomodoro.c config/pomodoro_config.c config/device_config.c
GPKRC_ENABLE = yes