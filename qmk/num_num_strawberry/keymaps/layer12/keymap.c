#include "../layer12.c"

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { clockwise ? encoder_cw() : encoder_ccw(); }
    return true;
}
