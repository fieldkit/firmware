#pragma once

#include "pool.h"
#include "hal/modmux.h"

#include "modules/registry.h"
#include "modules/scanning.h"
#include "modules/bridge/modules_bridge.h"

namespace fk {

struct ConstructedModule {
    FoundModule found;
    ModuleMetadata const *meta;
    Module *module;
};

using ConstructedModulesCollection = std::list<ConstructedModule, pool_allocator<ConstructedModule>>;

class ModuleFactory {
private:
    MallocPool pool_{ "modules", 8192 };
    ConstructedModulesCollection modules_{ pool_ };

public:
    ModuleFactory();
    virtual ~ModuleFactory();

public:
    tl::expected<ConstructedModulesCollection, Error> create(ModuleScanning &scanning, ScanningContext &ctx, Pool &pool);

private:
    bool recreate(ScanningContext &ctx, FoundModuleCollection module_headers);

};

ModuleFactory &get_module_factory();

}
