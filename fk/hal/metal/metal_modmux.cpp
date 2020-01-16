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

constexpr uint8_t to_mux_position(uint8_t p) {
    return p * 2;
}

#if defined(FK_TOPOLOGY_CHANGES)
static void topology_irq() {
    reinterpret_cast<MetalModMux *>(get_modmux())->irq();
}
#endif

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
        (uint8_t)0b00000000, // IPOL
        (uint8_t)0b00000000, // GPINTEN
        (uint8_t)0b00000000, // DEFVAL
        (uint8_t)0b00000000, // INTCON
        (uint8_t)0b00000000, // IOCON
        (uint8_t)0b00000000, // GPPU
        (uint8_t)0b00000000, // INTF
        (uint8_t)0b00000000, // INTCAP
        (uint8_t)gpio_,      // GPIO
    };

    bus.end();
    bus.begin();

    auto success = true;

    if (!I2C_CHECK(bus.write(MCP23008_ADDRESS, buffer, sizeof(buffer)))) {
        logerror("mcp23008 unresponsive");
        success = false;
    }

    if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 0))) {
        logerror("tca9548a unresponsive");
        success = false;
    }

    if (success) {
        loginfo("started, all modules off...");
        available_ = true;

        // Setup IRQ handler for getting notified about module topology changes.
        #if defined(FK_TOPOLOGY_CHANGES)
        pinMode(MODULE_SWAP, INPUT);
        auto irq = digitalPinToInterrupt(MODULE_SWAP);
        attachInterrupt(irq, topology_irq, CHANGE);
        #endif
    }

    return success;
}

bool MetalModMux::enable_topology_irq() {
    #if defined(FK_TOPOLOGY_CHANGES)
    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write_register_u8(MCP23008_ADDRESS, MCP23008_GPINTEN, 0b10101010))) {
        return false;
    }
    #endif

    return true;
}

bool MetalModMux::disable_topology_irq() {
    #if defined(FK_TOPOLOGY_CHANGES)
    auto bus = get_board()->i2c_module();

    if (!I2C_CHECK(bus.write_register_u8(MCP23008_ADDRESS, MCP23008_GPINTEN, 0b00000000))) {
        return false;
    }
    #endif

    return true;
}

optional<Topology> MetalModMux::read_topology_register() {
    auto bus = get_board()->i2c_module();

    uint8_t gpio = 0;
    if (!I2C_CHECK(bus.read_register_u8(MCP23008_ADDRESS, MCP23008_GPIO, gpio))) {
        return nullopt;
    }

    return { gpio };
}

bool MetalModMux::update_gpio(uint8_t new_gpio) {
    if (!available_) {
        return false;
    }

    if (fk_config().modules_always_on) {
        new_gpio = 0b01010101;
    }

    if (gpio_ == new_gpio) {
        return true;
    }

    logdebug("modmux.gpio = %d", new_gpio);

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
    auto new_gpio = 0b01010101;

    loginfo("all modules on (0x%02x)", new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::disable_all_modules() {
    auto new_gpio = 0x00;

    loginfo("all modules off (0x%02x)", new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::enable_module(uint8_t position) {
    auto mux_position = to_mux_position(position);
    auto new_gpio = gpio_ | (1 << mux_position);

    loginfo("[%d] module on (0x%02x)", mux_position, new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::disable_module(uint8_t position) {
    auto mux_position = to_mux_position(position);
    auto new_gpio = gpio_ & ~(1 << mux_position);

    loginfo("[%d] module off (0x%02x)", mux_position, new_gpio);

    return update_gpio(new_gpio);
}

bool MetalModMux::power_cycle(uint8_t position) {
    if (!available_) {
        return false;
    }

    if (!disable_module(position)) {
        return false;
    }

    fk_delay(1000);

    if (!enable_module(position)) {
        return false;
    }

    return true;
}

bool MetalModMux::choose(uint8_t position) {
    if (position == VirtualPosition) {
        if (!available_) {
            return true;
        }
        return choose_nothing();
    }

    if (!available_) {
        return false;
    }

    auto bus = get_board()->i2c_module();

    if (active_module_ == position) {
        return true;
    }

    auto mux_position = to_mux_position(position);

    logtrace("[%d] selecting", mux_position);

    bus.end();
    bus.begin();

    for (auto i = 0; i < 3; ++i) {
        if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 0))) {
            logwarn("choose nothing fail");
        }

        if (!I2C_CHECK(bus.write_u8(TCA9548A_ADDRESS, 1 << mux_position))) {
            logwarn("choose %d fail", mux_position);
            continue;
        }
        else {
            if (i > 0) {
                logwarn("choose took %d tries", i);
            }
            break;
        }
    }

    active_module_ = position;

    return true;
}

bool MetalModMux::choose_nothing() {
    if (!available_) {
        return true;
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

void MetalModMux::irq() {
    if (!change_.valid()) {
        change_ = { fk_uptime() };

        if (!get_ipc()->enqueue_activity(&change_)) {
            logerror("ipc error (activity)");
        }
    }
}

ModulesLock MetalModMux::lock() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);
    auto eeprom_lock = get_board()->lock_eeprom();

    FK_ASSERT(modules_lock);

    disable_topology_irq();

    return { std::move(modules_lock), std::move(eeprom_lock), fk_uptime() };
}

} // namespace fk

#endif
