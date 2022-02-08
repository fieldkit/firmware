#pragma once

#include "modules/bridge/modules.h"
namespace fk {
class EnableModulePower {
private:
    ModulePosition position_;
    ModulePower power_;
    uint32_t wake_delay_{ 0 };
    uint32_t enabled_at_{ 0 };

public:
    EnableModulePower(ModulePosition position, ModulePower power, uint32_t wake_delay);
    virtual ~EnableModulePower();

private:
    bool can_control();

    bool enabled_once();

    bool always_enabled();

    bool should_enable();

    bool was_enabled();

public:
    bool enable();

    void fatal_error();
};

} // namespace fk
