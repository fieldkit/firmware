#pragma once

#include "common.h"
#include "containers.h"
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

class StaticModuleScanning : public ModuleScanning {
private:
    FoundModuleCollection &found_;

public:
    StaticModuleScanning(FoundModuleCollection &found) : ModuleScanning(nullptr), found_(found) {
    }

public:
    nonstd::optional<FoundModuleCollection> scan(Pool &pool) override {
        return found_;
    }
};

}
