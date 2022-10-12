#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "modules/eeprom.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("modmux");

PinModMux::PinModMux() {
}

bool PinModMux::begin() {
    return true;
}

bool PinModMux::enable_topology_irq() {
    return true;
}

bool PinModMux::disable_topology_irq() {
    return true;
}

optional<Topology> PinModMux::read_topology_register() {
    return { 0 };
}

bool PinModMux::enable_all_modules() {
    return true;
}

bool PinModMux::disable_all_modules() {
    return true;
}

bool PinModMux::enable_module(ModulePosition position, ModulePower power) {
    return true;
}

bool PinModMux::disable_module(ModulePosition position) {
    return true;
}

bool PinModMux::disable_modules(ModulePower power) {
    return true;
}

bool PinModMux::power_cycle(ModulePosition position) {
    return true;
}

bool PinModMux::choose(ModulePosition position) {
    return true;
}

bool PinModMux::read_eeprom(uint32_t address, uint8_t *data, size_t size) {
    return true;
}

bool PinModMux::choose_nothing() {
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
