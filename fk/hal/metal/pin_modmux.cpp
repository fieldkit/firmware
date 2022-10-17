#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "modules/eeprom.h"

#if defined(__SAMD51__)

namespace fk {

const uint8_t ModulePowerPins[] = { 56u, 68u, 61u, 60u };

FK_DECLARE_LOGGER("pinmodmux");

PinModMux::PinModMux() {
}

bool PinModMux::begin() {
    available_ = true;
    enabled_ = -1;

    loginfo("begin");

    for (auto pin : ModulePowerPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    return true;
}

bool PinModMux::enable_topology_irq() {
    return false;
}

bool PinModMux::disable_topology_irq() {
    return false;
}

optional<Topology> PinModMux::read_topology_register() {
    return { 0 };
}

bool PinModMux::enable_all_modules() {
    return false;
}

bool PinModMux::disable_all_modules() {
    return choose_nothing();
}

bool PinModMux::enable_module(ModulePosition position, ModulePower power) {
    if (position == ModulePosition::Virtual) {
        return choose_nothing();
    }

    if (!disable_all_modules()) {
        logwarn("diasble all failed");
    }

    if (position.integer() >= MaximumNumberOfPhysicalModules) {
        logerror("module position out of bounds");
        return false;
    }

    logdebug("[%d] selecting", position.integer());
    digitalWrite(ModulePowerPins[position.integer()], HIGH);

    fk_delay(100);

    return true;
}

bool PinModMux::disable_module(ModulePosition position) {
    if (enabled_ >= 0 && enabled_ != position.integer()) {
        logwarn("[%d] disabled while inactive");
    }

    enabled_ = -1;

    return disable_all_modules();
}

bool PinModMux::disable_modules(ModulePower power) {
    return disable_all_modules();
}

bool PinModMux::power_cycle(ModulePosition position) {
    if (!disable_module(position)) {
        return false;
    }

    fk_delay(1000); // Too long?

    if (!enable_module(position, ModulePower::ReadingsOnly)) {
        return false;
    }

    return true;
}

bool PinModMux::choose(ModulePosition position) {
    return enable_module(position, ModulePower::ReadingsOnly);
}

bool PinModMux::read_eeprom(uint32_t address, uint8_t *data, size_t size) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };
    if (!eeprom.read_data(address, data, size)) {
        return false;
    }

    return true;
}

bool PinModMux::choose_nothing() {
    for (auto pin : ModulePowerPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }

    return true;
}

ModulesLock PinModMux::lock() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);
    auto eeprom_lock = get_board()->lock_eeprom();

    FK_ASSERT(modules_lock);

    return { std::move(modules_lock), std::move(eeprom_lock), fk_uptime() };
}

bool PinModMux::any_modules_on(ModulePower power) {
    return false;
}

bool PinModMux::is_module_on(ModulePosition position) {
    return false;
}

} // namespace fk

#endif
