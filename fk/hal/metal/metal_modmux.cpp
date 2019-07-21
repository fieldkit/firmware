#include "hal/hal.h"
#include "hal/metal/metal.h"

#if defined(ARDUINO)

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("modmux");

constexpr uint8_t Mcp23008Address = 0x20;

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

    Wire2.beginTransmission(Mcp23008Address);
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
    if (Wire2.endTransmission() != 0) {
        return false;
    }

    available_ = true;

    return true;
}

bool MetalModMux::enable_all_modules() {
    if (!available_) {
        return false;
    }

    Wire2.beginTransmission(Mcp23008Address);
    Wire2.write((byte)MCP23008_GPIO);
    Wire2.write((byte)0xff);
    if (Wire2.endTransmission() != 0) {
        return false;
    }

    return true;
}

bool MetalModMux::disable_all_modules() {
    if (!available_) {
        return false;
    }

    Wire2.beginTransmission(Mcp23008Address);
    Wire2.write((byte)MCP23008_GPIO);
    Wire2.write((byte)0x00);
    if (Wire2.endTransmission() != 0) {
        return false;
    }

    return true;
}

}

#endif
