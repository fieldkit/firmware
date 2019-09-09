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

    // All modules off.
    gpio_ = 0;
    active_module_ = NoModuleSelected;

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
        (uint8_t)gpio_,      // GPIO
    };

    bus.end();
    bus.begin();

    if (!I2C_CHECK(bus.write(MCP23008_ADDRESS, buffer, sizeof(buffer)))) {
        return false;
    }

    loginfo("started, all modules off...");

    available_ = true;

    if (!choose_nothing()) {
        return false;
    }

    return true;
}

bool MetalModMux::update_gpio(uint8_t new_gpio) {
    if (!available_) {
        return false;
    }

    if (gpio_ == new_gpio) {
        return true;
    }

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    if (!I2C_CHECK(bus.write_register_u8(MCP23008_ADDRESS, MCP23008_GPIO, new_gpio))) {
        return false;
    }

    gpio_ = new_gpio;

    return true;
}

bool MetalModMux::enable_all_modules() {
    auto new_gpio = 0xff;

    logtrace("all modules on (0x%02x)", new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::disable_all_modules() {
    auto new_gpio = 0x00;

    logtrace("all modules off (0x%02x)", new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::enable_module(uint8_t position) {
    auto new_gpio = gpio_ | (1 << position);

    logtrace("[%d] module on (0x%02x)", position, new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::disable_module(uint8_t position) {
    auto new_gpio = gpio_ & ~(1 << position);

    logtrace("[%d] module off (0x%02x)", position, new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::choose(uint8_t position) {
    if (!available_) {
        return false;
    }

    if (position == VirtualPosition) {
        return choose_nothing();
    }

    if (active_module_ == position) {
        return true;
    }

    logtrace("[%d] selecting", position);

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 0))) {
        logerror("choose nothing fail");
        return false;
    }

    fk_delay(10);

    if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 1 << position))) {
        logerror("choose %d fail", position);
        return false;
    }

    fk_delay(10);

    active_module_ = position;

    return true;
}

bool MetalModMux::choose_nothing() {
    if (!available_) {
        return false;
    }

    if (active_module_ == NoModuleSelected) {
        return true;
    }

    logtrace("[-] deselecting");

    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 0))) {
        return false;
    }

    active_module_ = NoModuleSelected;

    return true;
}

}

#endif
