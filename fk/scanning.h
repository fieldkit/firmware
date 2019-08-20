#pragma once

#include "common.h"
#include "config.h"
#include "hal/modmux.h"
#include "modules/modules.h"
#include "pool.h"

namespace fk {

struct FoundModule {
    uint8_t position;
    ModuleHeader header;
};

using FoundModuleCollection = std::list<FoundModule, pool_allocator<FoundModule>>;

class ModuleScanning {
private:
    ModMux *mm_;

public:
    ModuleScanning(ModMux *mm);

public:
    virtual nonstd::optional<FoundModuleCollection> scan(Pool &pool);
    virtual bool configure(uint8_t position, ModuleHeader &header);

private:
    bool available();

};

}
