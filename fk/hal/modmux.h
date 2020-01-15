#pragma once

#include "hal/board.h"
#include "activity.h"

namespace fk {

class ModulesLock {
private:
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

struct TopologyChange : public Activity {
    uint32_t time;

    TopologyChange() {
    }

    TopologyChange(uint32_t time) : time(time) {
    }

    bool valid() const {
        return time > 0;
    }

    void consumed() override;

};

struct Topology {
    bool success;
    uint8_t value;

    operator bool() const {
        return success;
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
    virtual Topology refresh_topology() = 0;
    virtual ModulesLock lock() = 0;

public:
    bool available() const {
        return available_;
    }

};

ModMux *get_modmux();

}
