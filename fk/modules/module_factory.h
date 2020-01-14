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
    ModuleStatus status;
};

using ConstructedModulesCollection = collection<ConstructedModule>;

class ModuleFactory {
private:
    StandardPool pool_{ "modules" };
    ConstructedModulesCollection modules_{ pool_ };
    uint32_t service_interval_{ 0 };

public:
    ModuleFactory();
    virtual ~ModuleFactory();

public:
    void clear();

    tl::expected<ConstructedModule, Error> get(uint8_t bay);

    tl::expected<ConstructedModulesCollection, Error> create(ModuleScanning &scanning, ScanningContext &ctx, Pool &pool);

    uint32_t service_interval() const {
        return service_interval_;
    }

    bool service(ScanningContext &ctx, Pool &pool);

private:
    bool recreate(ScanningContext &ctx, FoundModuleCollection &module_headers);

};

ModuleFactory &get_module_factory();

}
