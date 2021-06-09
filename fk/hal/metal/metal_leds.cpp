#include "hal/metal/metal_leds.h"

namespace fk {

FK_DECLARE_LOGGER("leds");

constexpr uint8_t LP50_ADDRESS = 0x14;
constexpr uint8_t LP50_REGISTER_ENABLE = 0x00;
constexpr uint8_t LP50_REGISTER_RGB = 0x0B;
constexpr uint8_t LP50_REGISTER_BRIGHTNESS = 0x07;
constexpr uint8_t LP50_ENABLE_ON = 0x40;

MetalLeds::MetalLeds() {
    bzero(pixels_, sizeof(pixels_));
    for (auto i = 0u; i < 1u; ++i) {
        pixels_[i][0] = LP50_REGISTER_BRIGHTNESS;
        for (auto j = 0u; j < NumberOfPixels; ++j) {
            pixels_[i][1 + j] = 0xff;
        }
    }
}

bool MetalLeds::begin() {
    auto bus = get_board()->i2c_module();

    status_ = Availability::Unavailable;

    if (!I2C_CHECK(bus.write_register_u8(LP50_ADDRESS, LP50_REGISTER_ENABLE, LP50_ENABLE_ON))) {
        return false;
    }

    off();

    status_ = Availability::Available;

    return true;
}

bool MetalLeds::brightness(uint8_t value, bool refresh_after) {
    auto ptr = &pixels_[active_][1];
    for (auto i = 0u; i < NumberOfPixels; ++i) {
        if (*ptr != value) {
            dirty_ = true;
        }
        *ptr++ = value;
    }

    if (dirty_) {
        if (NumberOfPixels == 4) {
            loginfo("bness-all = [%d, %d, %d, %d]", ptr[0], ptr[1], ptr[2], ptr[3]);
        }
    }

    if (refresh_after) {
        refresh();
    }

    return dirty_;
}

void MetalLeds::off() {
    if (brightness(0x00)) {
        loginfo("off");
    }
}

void MetalLeds::on() {
    if (brightness(0xff)) {
        loginfo("on");
    }
}

void MetalLeds::color(uint8_t position, Color color, bool refresh_after) {
    auto ptr = &pixels_[active_][1 + NumberOfPixels + (position * 3)];

    dirty_ = ptr[0] != color.b || ptr[1] != color.g || ptr[2] != color.r;

    ptr[0] = color.b;
    ptr[1] = color.g;
    ptr[2] = color.r;

    if (dirty_) {
        loginfo("[%d] color = %02x%02x%02x", position, color.r, color.g, color.b);
    }

    if (refresh_after) {
        refresh();
    }
}

void MetalLeds::off(uint8_t position) {
    auto ptr = &pixels_[active_][1 + position];
    dirty_ = *ptr != 0x00;
    if (dirty_) {
        loginfo("[%d] off", position);
    }
    *ptr = 0x00;

    refresh();
}

void MetalLeds::on(uint8_t position) {
    auto ptr = &pixels_[active_][1 + position];
    dirty_ = *ptr != 0xff;
    if (dirty_) {
        loginfo("[%d] on", position);
    }
    *ptr = 0xff;

    refresh();
}

bool MetalLeds::refresh() {
    if (status_ == Availability::Unavailable) {
        return false;
    }

    if (dirty_) {
        loginfo("refresh");
    }

    dirty_ = false;

    auto bus = get_board()->i2c_module();
    if (!I2C_CHECK(bus.write(LP50_ADDRESS, pixels_[active_], sizeof(pixels_[active_])))) {
        logerror("refresh error");
        return false;
    }

    return true;
}

} // namespace fk
