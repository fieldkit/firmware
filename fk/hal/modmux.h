#pragma once

#include "hal/board.h"
#include "hal/mutex.h"
#include "activity.h"

namespace fk {

class ModulesLock {
private:
    Lock lock_;
    EepromLock eeprom_;
    uint32_t locked_;

public:
    explicit ModulesLock();
    explicit ModulesLock(ModulesLock const &o);
    ModulesLock(EepromLock eeprom, uint32_t locked);
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

public:
    Topology() {
    }

    Topology(uint8_t value) : value_(value) {
    }

public:
    uint8_t value() const {
        return value_;
    }

    const char *string(char *buffer, size_t size) const;

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
    virtual optional<Topology> refresh_topology() = 0;
    virtual ModulesLock lock() = 0;

public:
    bool available() const {
        return available_;
    }

};

ModMux *get_modmux();

}
