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
    Wire2.begin();

    // Configure IO directions and disable everything.
    Wire2.beginTransmission(MCP23008_ADDRESS);
    Wire2.write((byte)MCP23008_IODIR);
    Wire2.write((byte)0b10101010); // IODIR
    Wire2.write((byte)0x00);       // IPOL
    Wire2.write((byte)0x00);       // GPINTEN
    Wire2.write((byte)0x00);       // DEFVAL
    Wire2.write((byte)0x00);       // INTCON
    Wire2.write((byte)0x00);       // IOCON
    Wire2.write((byte)0x00);       // GPPU
    Wire2.write((byte)0x00);       // INTF
    Wire2.write((byte)0x00);       // INTCAP
    Wire2.write((byte)0x00);       // GPIO
    if (!I2C_CHECK(Wire2.endTransmission())) {
        logwarn("mcp23008 error, missing backplane?");
        return false;
    }

    available_ = true;

    return true;
}

bool MetalModMux::enable_all_modules() {
    if (!available_) {
        return false;
    }

    Wire2.beginTransmission(MCP23008_ADDRESS);
    Wire2.write((byte)MCP23008_GPIO);
    Wire2.write((byte)0xff);
    if (!I2C_CHECK(Wire2.endTransmission())) {
        return false;
    }

    return true;
}

bool MetalModMux::disable_all_modules() {
    if (!available_) {
        return false;
    }

    Wire2.beginTransmission(MCP23008_ADDRESS);
    Wire2.write((byte)MCP23008_GPIO);
    Wire2.write((byte)0x00);
    if (!I2C_CHECK(Wire2.endTransmission())) {
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

    Wire2.beginTransmission(TCA9548A_ADDRESS);
    Wire2.write(1 << position);
    if (!I2C_CHECK(Wire2.endTransmission())) {
        return false;
    }

    return true;
}

}

#endif
