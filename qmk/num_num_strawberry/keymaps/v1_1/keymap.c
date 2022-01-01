#include "../default.c"

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { clockwise ? encoder_cw(is_hold) : encoder_ccw(is_hold); }
    return true;
}
