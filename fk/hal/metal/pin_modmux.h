#pragma once

#include "hal/hal.h"

namespace fk {

class PinModMux : public ModMux {
private:
    ModulePosition enabled_{ ModulePosition::None };

public:
    PinModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;
    bool enable_module(ModulePosition position, ModulePower power) override;
    bool disable_module(ModulePosition position) override;
    bool disable_modules(ModulePower power) override;
    bool power_cycle(ModulePosition position) override;
    bool choose(ModulePosition position) override;
    bool choose_nothing() override;
    bool enable_topology_irq() override;
    bool disable_topology_irq() override;
    optional<Topology> read_topology_register() override;
    ModulesLock lock() override;
    bool any_modules_on(ModulePower power) override;
    bool is_module_on(ModulePosition position) override;
    bool read_eeprom(uint32_t address, uint8_t *data, size_t size) override;
};

} // namespace fk
