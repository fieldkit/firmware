#include "exchange.h"

#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

FK_DECLARE_LOGGER("modmux");

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
        get_modmux()->enable_topology_irq();
        get_board()->release_eeprom();
    }
}

TopologyChange::TopologyChange() {
}

TopologyChange::TopologyChange(uint32_t created) : Activity(created) {
}

void TopologyChange::consumed() {
    Activity::consumed();

    auto topology = get_modmux()->refresh_topology();
    if (!topology) {
        logerror("error refreshing topo");
        return;
    }

    if (false) {
        #pragma GCC diagnostic ignored "-Wformat"
        #pragma GCC diagnostic ignored "-Wformat-extra-args"
        loginfo("topology: %b", topology->value());
        #pragma GCC diagnostic pop
        #pragma GCC diagnostic pop
    }
    else {
        char buffer[32];
        loginfo("topology: [%s]", topology->string(buffer, sizeof(buffer)));
    }
}

const char *Topology::string(char *buffer, size_t size) const {
    auto ptr = buffer;
    for (auto i = 0u; i < 4u; ++i) {
        if (value_ & (1 << ((i * 2) + 1))) {
            *ptr = '#';
        }
        else {
            *ptr = ' ';
        }
        ptr++;
    }
    *ptr = 0;
    return buffer;
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
