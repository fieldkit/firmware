#include "hal/metal/metal_leds.h"

namespace fk {

constexpr uint8_t LP50_ADDRESS = 0x14;
constexpr uint8_t LP50_REGISTER_ENABLE = 0x00;
constexpr uint8_t LP50_REGISTER_RGB = 0x0B;
constexpr uint8_t LP50_REGISTER_BRIGHTNESS = 0x07;
constexpr uint8_t LP50_ENABLE_ON = 0x40;

MetalLeds::MetalLeds() {
    bzero(pixels_, sizeof(pixels_));
    pixels_[0] = LP50_REGISTER_BRIGHTNESS;
    for (auto i = 0u; i < NumberOfPixels; ++i) {
        pixels_[1 + i] = 0xff;
    }
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
    for (auto i = 0u; i < NumberOfPixels; ++i) {
        pixels_[1 + i] = value;
    }

    refresh();
}

void MetalLeds::off() {
    brightness(0x00);
}

void MetalLeds::on() {
    brightness(0xff);
}

void MetalLeds::color(uint8_t position, Color color) {
    pixels_[1 + NumberOfPixels + (position * 3) + 0] = color.r;
    pixels_[1 + NumberOfPixels + (position * 3) + 1] = color.g;
    pixels_[1 + NumberOfPixels + (position * 3) + 2] = color.g;

    refresh();
}

void MetalLeds::off(uint8_t position) {
    pixels_[1 + position] = 0x00;

    refresh();
}

void MetalLeds::on(uint8_t position) {
    pixels_[1 + position] = 0xff;

    refresh();
}

bool MetalLeds::refresh() {
    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write(LP50_ADDRESS, pixels_, sizeof(pixels_)))) {
        return false;
    }

    return true;
}

} // namespace fk
