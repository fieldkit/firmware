#pragma once

#include "common.h"
#include "collections.h"
#include "config.h"
#include "hal/modmux.h"
#include "pool.h"
#include "modules/shared/modules.h"

namespace fk {

struct FoundModule {
    ModulePosition position;
    ModuleHeader header;

    bool physical() const {
        return position != ModMux::VirtualPosition;
    }
};

using FoundModuleCollection = collection<FoundModule>;

class ModuleScanning {
private:
    ModMux *mm_;

public:
    explicit ModuleScanning(ModMux *mm);

public:
    virtual tl::expected<FoundModuleCollection, Error> scan(Pool &pool);
    virtual bool configure(ModulePosition position, ModuleHeader &header);
    virtual bool erase(ModulePosition position);

private:
    bool available();
    bool try_scan_single_module(ModulePosition position, FoundModuleCollection &found, Pool &pool);

};

class StaticModuleScanning : public ModuleScanning {
private:
    FoundModuleCollection &found_;

public:
    explicit StaticModuleScanning(FoundModuleCollection &found) : ModuleScanning(nullptr), found_(found) {
    }

public:
    tl::expected<FoundModuleCollection, Error> scan(Pool &pool) override {
        return FoundModuleCollection(found_);
    }
};

} // namespace fk
