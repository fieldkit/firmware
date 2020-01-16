#include "exchange.h"

#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

FK_DECLARE_LOGGER("modmux");

optional<Topology> ModMux::get_topology() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);

    FK_ASSERT(modules_lock);

    if (!choose_nothing()) {
        return nullopt;
    }

    return read_topology_register();
}

ModulesLock::ModulesLock() {
}

ModulesLock::ModulesLock(ModulesLock const &o) : eeprom_(o.eeprom_), locked_(o.locked_) {
}

ModulesLock::ModulesLock(Lock lock, EepromLock eeprom, uint32_t locked) : lock_(std::move(lock)), eeprom_(std::move(eeprom)), locked_(locked) {
}

ModulesLock::ModulesLock(ModulesLock &&o) : lock_(std::move(o.lock_)), eeprom_(std::move(o.eeprom_)), locked_(exchange(o.locked_, 0)) {
}

ModulesLock::~ModulesLock() {
    if (locked_ > 0) {
        get_modmux()->enable_topology_irq();
    }
}

TopologyChange::TopologyChange() {
}

TopologyChange::TopologyChange(uint32_t created) : Activity(created) {
}

void TopologyChange::consumed() {
    Activity::consumed();

    auto topology = get_modmux()->read_topology_register();
    if (!topology) {
        logerror("error refreshing topo");
        return;
    }

    loginfo("topology: [%s]", topology->string());
}

Topology::Topology() : value_(0) {
    string_[0] = 0;
}

Topology::Topology(uint8_t value) : value_(value) {
    FK_ASSERT(sizeof(string_) >= (MaximumNumberOfPhysicalModules + 1));

    auto ptr = string_;
    for (auto i = 0u; i < 4u; ++i) {
        if (value_ & (1 << ((i * 2) + 1))) {
            *ptr = '1' + i;
        }
        else {
            *ptr = ' ';
        }
        ptr++;
    }
    *ptr = 0;
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
