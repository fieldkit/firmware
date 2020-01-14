#include "exchange.h"

#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

ModulesLock::ModulesLock() {
}

ModulesLock::ModulesLock(ModulesLock const &o) : eeprom_(o.eeprom_), locked_(o.locked_) {
}

ModulesLock::ModulesLock(EepromLock eeprom, uint32_t locked) : eeprom_(std::move(eeprom)), locked_(locked) {
}

ModulesLock::ModulesLock(ModulesLock &&o) : eeprom_(std::move(o.eeprom_)), locked_(exchange(o.locked_, 0)) {
}

ModulesLock::~ModulesLock() {
    if (locked_ > 0) {
        get_board()->release_eeprom();
    }
}

#if defined(FK_HARDWARE_FULL)
MetalModMux mm;
#else
LinuxModMux mm;
#endif

ModMux *get_modmux() {
    return &mm;
}

}
