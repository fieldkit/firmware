#include "modules/bridge/modules.h"

namespace fk {

class EnableModulePower {
private:
    bool enabled_;
    ModulePower power_;
    ModulePosition position_;
    bool was_enabled_{ false };

public:
    EnableModulePower(bool enabled, ModulePower power, ModulePosition position) : enabled_(enabled), power_(power), position_(position) {
    }

    virtual ~EnableModulePower();

private:
    bool can_control();

    bool enabled_once();

    bool always_enabled();

    bool should_enable();

public:
    bool enable();

    void fatal_error();

    bool was_enabled() {
        return was_enabled_;
    }

};

} // namespace fk
