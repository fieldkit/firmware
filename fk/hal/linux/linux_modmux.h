#pragma once

#include "hal/hal.h"

namespace fk {

class LinuxModMux : public ModMux {
public:
    LinuxModMux();

public:
    bool begin() override;
    bool enable_all_modules() override;
    bool disable_all_modules() override;
    bool enable_module(uint8_t position) override;
    bool disable_module(uint8_t position) override;
    bool power_cycle(uint8_t position) override;
    bool choose(uint8_t position) override;
    bool choose_nothing() override;
    bool enable_topology_irq() override;
    bool disable_topology_irq() override;
    Topology refresh_topology() override;
    ModulesLock lock() override;

};

}
