#pragma once

#include "hal/board.h"
#include "hal/mutex.h"
#include "activity.h"
#include "config.h"

namespace fk {

typedef uint8_t ModulePosition;

static inline uint8_t module_position_display(ModulePosition position) {
    return (uint8_t)position;
}

static inline ModulePosition module_position_from(uint8_t i) {
    return (ModulePosition)i;
}

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
    char string_[(sizeof(uint8_t) * 8) + 1];

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

    bool all_modules_on() const;

};

class ModMux {
public:
    constexpr static ModulePosition VirtualPosition = 0xff;
    constexpr static ModulePosition NoModuleSelected = 0xff;
    constexpr static ModulePosition AllModules = 0xff;

protected:
    bool available_;
    TopologyChange change_;

public:
    virtual bool begin() = 0;
    virtual bool enable_all_modules() = 0;
    virtual bool disable_all_modules() = 0;
    virtual bool enable_module(ModulePosition position) = 0;
    virtual bool disable_module(ModulePosition position) = 0;
    virtual bool power_cycle(ModulePosition position) = 0;
    virtual bool choose(ModulePosition position) = 0;
    virtual bool choose_nothing() = 0;
    virtual bool enable_topology_irq() = 0;
    virtual bool disable_topology_irq() = 0;
    virtual optional<Topology> read_topology_register() = 0;
    virtual ModulesLock lock() = 0;

public:
    class iterator {
    private:
        uint8_t iter_;

    public:
        iterator(uint8_t iter) : iter_(iter) {
        }

    public:
        iterator operator++() {
            iter_++;
            return *this;
        }

        bool operator!=(const iterator &other) const {
            return iter_ != other.iter_;
        }

        ModulePosition operator*() const {
            return module_position_from(iter_);
        }
    };

    class all_modules {
    public:
        iterator begin() const {
            return iterator(0);
        }

        iterator end() const {
            return iterator(MaximumNumberOfPhysicalModules);
        }
    };

public:
    all_modules all() {
        return { };
    }

public:
    optional<Topology> get_topology();

    bool check_modules();

    bool available() const {
        return available_;
    }

};

ModMux *get_modmux();

}
