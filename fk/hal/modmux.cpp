#include "exchange.h"

#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "hal/linux/linux.h"

namespace fk {

FK_DECLARE_LOGGER("modmux");

const ModulePosition ModulePosition::Virtual{ 0xff };
const ModulePosition ModulePosition::None{ 0xff };
const ModulePosition ModulePosition::All{ 0xff };
const ModulePosition ModulePosition::Solo{ 0x00 };

optional<Topology> ModMux::get_topology() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);

    FK_ASSERT(modules_lock);

    if (!choose_nothing()) {
        return nullopt;
    }

    return read_topology_register();
}

bool ModMux::check_modules() {
    if (!available_) {
        return false;
    }

    auto topology = read_topology_register();
    if (!topology) {
        logwarn("unable to read topology");
        return false;
    }

    loginfo("topology: [%s]", topology->string());

    if (topology->all_modules_on()) {
        return true;
    }

    logerror("weird modules power off issue!");

    if (!enable_all_modules()) {
        logwarn("unable to enable modules");
    }

    auto topology_after = read_topology_register();
    if (!topology_after) {
        logwarn("unable to read topology");
        return false;
    }

    loginfo("topology: [%s]", topology_after->string());

    return false;
}

ModulesLock::ModulesLock() {
}

ModulesLock::ModulesLock(ModulesLock const &o) : eeprom_(o.eeprom_), locked_(o.locked_) {
}

ModulesLock::ModulesLock(Lock lock, EepromLock eeprom, uint32_t locked)
    : lock_(std::move(lock)), eeprom_(std::move(eeprom)), locked_(locked) {
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
            *ptr = '0' + i;
        } else {
            *ptr = ' ';
        }
        ptr++;
    }
    for (auto i = 0u; i < 4u; ++i) {
        if (value_ & (1 << ((i * 2) + 0))) {
            *ptr = '0' + i;
        } else {
            *ptr = ' ';
        }
        ptr++;
    }
    *ptr = 0;
}

bool Topology::all_modules_on() const {
    auto mask = 0b01010101;
    return (value_ & mask) == mask;
}

#if defined(FK_HARDWARE_FULL)
#if defined(FK_UNDERWATER)
PinModMux mm;
#else
MetalModMux mm;
#endif
#else
LinuxModMux mm;
#endif

ModMux *get_modmux() {
    return &mm;
}

} // namespace fk
