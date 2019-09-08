#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(ARDUINO)

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("modmux");

constexpr uint8_t MCP23008_ADDRESS = 0x20;
constexpr uint8_t TCA9548A_ADDRESS = 0x70;

constexpr uint8_t MCP23008_IODIR = 0x00;
constexpr uint8_t MCP23008_IPOL = 0x01;
constexpr uint8_t MCP23008_GPINTEN = 0x02;
constexpr uint8_t MCP23008_DEFVAL = 0x03;
constexpr uint8_t MCP23008_INTCON = 0x04;
constexpr uint8_t MCP23008_IOCON = 0x05;
constexpr uint8_t MCP23008_GPPU = 0x06;
constexpr uint8_t MCP23008_INTF = 0x07;
constexpr uint8_t MCP23008_INTCAP = 0x08;
constexpr uint8_t MCP23008_GPIO = 0x09;
constexpr uint8_t MCP23008_OLAT = 0x0A;

MetalModMux::MetalModMux() {
}

bool MetalModMux::begin() {
    auto bus = get_board()->i2c_module();

    uint8_t buffer[] = {
        (uint8_t)MCP23008_IODIR,
        (uint8_t)0b10101010, // IODIR
        (uint8_t)0x00,       // IPOL
        (uint8_t)0x00,       // GPINTEN
        (uint8_t)0x00,       // DEFVAL
        (uint8_t)0x00,       // INTCON
        (uint8_t)0x00,       // IOCON
        (uint8_t)0x00,       // GPPU
        (uint8_t)0x00,       // INTF
        (uint8_t)0x00,       // INTCAP
        (uint8_t)0x00,       // GPIO
    };

    bus.end();
    bus.begin();

    auto rv = bus.write(MCP23008_ADDRESS, buffer, sizeof(buffer));
    if (!I2C_CHECK(rv)) {
        return false;
    }

    available_ = true;

    return true;
}

bool MetalModMux::enable_all_modules() {
    if (!available_) {
        return false;
    }

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    auto rv = bus.write_register_u8(MCP23008_ADDRESS, MCP23008_GPIO, 0xff);
    if (!I2C_CHECK(rv)) {
        return false;
    }

    return true;
}

bool MetalModMux::disable_all_modules() {
    if (!available_) {
        return false;
    }

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    auto rv = bus.write_register_u8(MCP23008_ADDRESS, MCP23008_GPIO, 0x00);
    if (!I2C_CHECK(rv)) {
        return false;
    }

    return true;
}

bool MetalModMux::choose(uint8_t position) {
    if (position == ModMuxVirtualPosition) {
        return true;
    }

    if (!available_) {
        return false;
    }

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    auto rv = bus.write_u8(TCA9548A_ADDRESS, 1 << position);
    if (!I2C_CHECK(rv)) {
        return false;
    }

    return true;
}

}

#endif
