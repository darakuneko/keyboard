/*
Copyright 2020 Evy Dekkers

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

RGB_MATRIX_EFFECT(rainbow_beacon_all)


#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

bool effect_runner_sin_cos_all(effect_params_t* params, sin_cos_i_f effect_func) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint16_t time      = scale16by8(g_rgb_timer, rgb_matrix_config.speed / 4);
    int8_t   cos_value = cos8(time) - 128;
    int8_t   sin_value = sin8(time) - 128;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        RGB rgb = rgb_matrix_hsv_to_rgb(effect_func(rgb_matrix_config.hsv, cos_value, sin_value, i, time));
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return led_max < DRIVER_LED_TOTAL;
}

// Moving rainbow beacon all
static bool rainbow_beacon_all(effect_params_t* params) { 
    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min ; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], 0x02)) {
          rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
   }

    return effect_runner_sin_cos_all(params, &RAINBOW_BEACON_CUSTOM); 
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS