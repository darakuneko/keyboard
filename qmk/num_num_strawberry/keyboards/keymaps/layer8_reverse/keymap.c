#include "../layer8.c"

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { clockwise ? encoder_ccw() : encoder_cw(); }
    return true;
}
