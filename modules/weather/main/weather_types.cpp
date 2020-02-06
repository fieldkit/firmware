#include "weather_types.h"

namespace fk {

int16_t WindDirection::get_adc_value_from_raw_adc(uint32_t raw_adc) {
    return (__builtin_bswap16(raw_adc) >> 4) & 0xff;
}

int16_t WindDirection::get_angle_from_raw_adc(uint32_t raw_adc) {
    auto value = get_adc_value_from_raw_adc(raw_adc);

    if (value < 13) return 112;
    if (value < 16) return 67;
    if (value < 18) return 90;
    if (value < 25) return 157;
    if (value < 37) return 135;
    if (value < 50) return 202;
    if (value < 59) return 180;
    if (value < 86) return 22;
    if (value < 99) return 45;
    if (value < 133) return 247;
    if (value < 141) return 225;
    if (value < 161) return 337;
    if (value < 184) return 0;
    if (value < 196) return 292;
    if (value < 213) return 315;
    if (value < 231) return 270;

    return -1;
}

float WindDirection::get_mv_from_raw_adc(uint32_t raw_adc) {
    return get_adc_value_from_raw_adc(raw_adc) * (3.3f / 256.0f) * 1000.0f;
}

}
