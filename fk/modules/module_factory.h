#pragma once

#include "pool.h"
#include "hal/modmux.h"

#include "modules/registry.h"
#include "modules/scanning.h"
#include "modules/bridge/modules_bridge.h"

namespace fk {

class ModuleFactory {
private:
    StandardPool pool_{ "module-factory" };
    ConstructedModulesCollection modules_{ pool_ };
    uint32_t service_interval_{ 0 };

public:
    ModuleFactory();
    virtual ~ModuleFactory();

public:
    void clear();

    // TODO Remove this
    Pool &pool() {
        return pool_;
    }

    optional<ConstructedModule> get(ModulePosition bay);

    ConstructedModulesCollection modules();

    uint32_t service_interval() const;

    #if !defined(__SAMD51__)
    void modules(ConstructedModulesCollection modules);
    #endif

public:
    tl::expected<ConstructedModulesCollection, Error> rescan(ModuleScanning &scanning, Pool &pool);

    bool initialize(ScanningContext &ctx, Pool &pool);

    bool service(ScanningContext &ctx, Pool &pool);

    tl::expected<ConstructedModulesCollection, Error> rescan_and_initialize(ScanningContext ctx, ModuleScanning &scanning, Pool &pool);

private:
    bool changes(FoundModuleCollection &a, ConstructedModulesCollection &b);

    tl::expected<ConstructedModulesCollection, Error> resolve(FoundModuleCollection &module_headers, Pool &pool);

    void sort_modules();

};

ModuleFactory &get_module_factory();

} // namespace fk
