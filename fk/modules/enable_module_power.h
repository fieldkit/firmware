#include "modules/bridge/modules.h"

namespace fk {

class EnableModulePower {
private:
    bool enabled_;
    ModulePower power_;
    ModulePosition position_;

public:
    EnableModulePower(bool enabled, ModulePower power, ModulePosition position) : enabled_(enabled), power_(power), position_(position) {
    }

    virtual ~EnableModulePower();

public:
    bool enabled_once() {
        return enabled_ && position_ != ModulePosition::Virtual && power_ == ModulePower::ReadingsOnly;
    }

    bool always_enabled() {
        return power_ == ModulePower::Always;
    }

    bool enable();

};

} // namespace fk
