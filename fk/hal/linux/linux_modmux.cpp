#include "hal/hal.h"
#include "hal/linux/linux.h"

#if defined(linux)

namespace fk {

LinuxModMux::LinuxModMux() {
}

bool LinuxModMux::begin() {
    return true;
}

bool LinuxModMux::enable_all_modules() {
    return true;
}

bool LinuxModMux::disable_all_modules() {
    return true;
}

bool LinuxModMux::enable_module(ModulePosition position, ModulePower power) {
    return true;
}

bool LinuxModMux::disable_module(ModulePosition position) {
    return true;
}

bool LinuxModMux::disable_modules(ModulePower power) {
    return true;
}

bool LinuxModMux::power_cycle(ModulePosition position) {
    return true;
}

bool LinuxModMux::choose(ModulePosition position) {
    return true;
}

bool LinuxModMux::choose_nothing() {
    return true;
}

bool LinuxModMux::enable_topology_irq() {
    return true;
}

bool LinuxModMux::disable_topology_irq() {
    return true;
}

optional<Topology> LinuxModMux::read_topology_register() {
    return { 0 };
}

ModulesLock LinuxModMux::lock() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);
    auto eeprom_lock = get_board()->lock_eeprom();

    return { std::move(modules_lock), std::move(eeprom_lock), fk_uptime() };
}

bool LinuxModMux::any_modules_on(ModulePower power) {
    return false;
}

bool LinuxModMux::is_module_on(ModulePosition position) {
    return false;
}

}

#endif
