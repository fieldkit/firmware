#pragma once

#include "hal/hal.h"

namespace fk {

class MetalModMux : public ModMux {
private:
    uint8_t gpio_{ 0 };
    ModulePosition active_module_{ NoModuleSelected };

public:
    MetalModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;
    bool enable_module(ModulePosition position) override;
    bool disable_module(ModulePosition position) override;
    bool power_cycle(ModulePosition position) override;
    bool choose(ModulePosition position) override;
    bool choose_nothing() override;
    bool enable_topology_irq() override;
    bool disable_topology_irq() override;
    optional<Topology> read_topology_register() override;
    ModulesLock lock() override;

public:
    void irq();

private:
    bool update_gpio(uint8_t new_gpio);

};

}
