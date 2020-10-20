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
    bool enabled_once();

    bool always_enabled();

    bool enable();

};

} // namespace fk
