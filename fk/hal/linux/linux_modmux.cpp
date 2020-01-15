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

bool LinuxModMux::enable_module(uint8_t position) {
    return true;
}

bool LinuxModMux::disable_module(uint8_t position) {
    return true;
}

bool LinuxModMux::power_cycle(uint8_t position) {
    return true;
}

bool LinuxModMux::choose(uint8_t position) {
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

bool LinuxModMux::refresh_topology() {
    return true;
}

ModulesLock LinuxModMux::lock() {
    return { get_board()->lock_eeprom(), fk_uptime() };
}

}

#endif
