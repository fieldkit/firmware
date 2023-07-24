#pragma once

#include "common.h"
#include "hal/modmux.h"

namespace fk {

class ModuleLockout {
private:
    uint32_t unlocked_{ 0 };

public:
    bool can_enable();
    bool try_enable(ModulePosition position);
    void enable_until_uptime(uint32_t unlocked);
};

} // namespace fk