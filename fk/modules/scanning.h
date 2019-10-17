#pragma once

#include "common.h"
#include "containers.h"
#include "config.h"
#include "hal/modmux.h"
#include "pool.h"
#include "modules/shared/modules.h"

namespace fk {

struct FoundModule {
    uint8_t position;
    bool valid;
    ModuleHeader header;

    bool physical() const {
        return position != ModMux::VirtualPosition;
    }
};

using FoundModuleCollection = std::list<FoundModule, pool_allocator<FoundModule>>;

class ModuleScanning {
private:
    ModMux *mm_;

public:
    ModuleScanning(ModMux *mm);

public:
    virtual tl::expected<FoundModuleCollection, Error> scan(Pool &pool);
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
    tl::expected<FoundModuleCollection, Error> scan(Pool &pool) override {
        return found_;
    }
};

}
