#include "hal/metal/metal_leds.h"

namespace fk {

constexpr uint8_t LP50_ADDRESS = 0x14;
constexpr uint8_t LP50_REGISTER_ENABLE = 0x00;
constexpr uint8_t LP50_REGISTER_RGB = 0x0B;
constexpr uint8_t LP50_REGISTER_BRIGHTNESS = 0x07;
constexpr uint8_t LP50_ENABLE_ON = 0x40;

MetalLeds::MetalLeds() {
}

bool MetalLeds::begin() {
    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    if (!I2C_CHECK(bus.write_register_u8(LP50_ADDRESS, LP50_REGISTER_ENABLE, LP50_ENABLE_ON))) {
        return false;
    }

    off();

    return true;
}

void MetalLeds::brightness(uint8_t value) {
    uint8_t pixels[5] = {
        LP50_REGISTER_BRIGHTNESS,
        value, value, value, value
    };

    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write(LP50_ADDRESS, pixels, sizeof(pixels)))) {
    }
}

void MetalLeds::off() {
    uint8_t pixels[13] = {
        LP50_REGISTER_RGB,
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,
        0x00, 0x00, 0x00,
    };

    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write(LP50_ADDRESS, pixels, sizeof(pixels)))) {
    }
}

void MetalLeds::on() {
    uint8_t pixels[13] = {
        LP50_REGISTER_RGB,
        0xff, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xff, 0xff, 0xff,
        0xff, 0xff, 0xff,
    };

    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write(LP50_ADDRESS, pixels, sizeof(pixels)))) {

    }
}

} // namespace fk
