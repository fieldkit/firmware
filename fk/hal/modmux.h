#pragma once

#include "hal/board.h"
#include "hal/mutex.h"
#include "activity.h"
#include "config.h"

namespace fk {

class ModulesLock {
private:
    Lock lock_;
    EepromLock eeprom_;
    uint32_t locked_;

public:
    explicit ModulesLock();
    explicit ModulesLock(ModulesLock const &o);
    ModulesLock(Lock lock, EepromLock eeprom, uint32_t locked);
    ModulesLock(ModulesLock &&o);

public:
    virtual ~ModulesLock();

};

class TopologyChange : public Activity {
public:
    TopologyChange();
    TopologyChange(uint32_t created);

public:
    void consumed() override;

};

class Topology {
private:
    uint8_t value_;
    char string_[MaximumNumberOfPhysicalModules + 1];

public:
    Topology();
    Topology(uint8_t value);

public:
    bool operator ==(const Topology &b) const {
        return value_ == b.value_;
    }

    bool operator !=(const Topology &b) const {
        return value_ != b.value_;
    }

public:
    uint8_t value() const {
        return value_;
    }

    const char *string() const {
        return string_;
    }

};

class ModMux {
public:
    constexpr static uint8_t VirtualPosition = 0xff;
    constexpr static uint8_t NoModuleSelected = 0xff;

protected:
    bool available_;
    TopologyChange change_;

public:
    virtual bool begin() = 0;
    virtual bool enable_all_modules() = 0;
    virtual bool disable_all_modules() = 0;
    virtual bool enable_module(uint8_t position) = 0;
    virtual bool disable_module(uint8_t position) = 0;
    virtual bool power_cycle(uint8_t position) = 0;
    virtual bool choose(uint8_t position) = 0;
    virtual bool choose_nothing() = 0;
    virtual bool enable_topology_irq() = 0;
    virtual bool disable_topology_irq() = 0;
    virtual optional<Topology> read_topology_register() = 0;
    virtual ModulesLock lock() = 0;

public:
    optional<Topology> get_topology();

    bool available() const {
        return available_;
    }

};

ModMux *get_modmux();

}
