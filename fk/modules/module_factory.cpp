#include "module_factory.h"

#include "scanning.h"
#include "registry.h"

namespace fk {

FK_DECLARE_LOGGER("modules");

static ModuleFactory module_factory;

ModuleFactory &get_module_factory() {
    return module_factory;
}

ModuleFactory::ModuleFactory() {
}

ModuleFactory::~ModuleFactory() {
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::create(ModuleScanning &scanning, ScanningContext &ctx, Pool &pool) {
    auto module_headers = scanning.scan(pool);
    if (!module_headers) {
        logerror("error scanning modules");
        return tl::unexpected<Error>(module_headers.error());
    }

    ModuleRegistry registry;
    ConstructedModulesCollection modules(pool);
    for (auto &f : *module_headers) {
        auto meta = registry.resolve(f.header);
        if (meta != nullptr) {
            auto module = meta->ctor(pool);
            modules.emplace_back(ConstructedModule{
                    .found = f,
                    .meta = meta,
                    .module = module,
                });
        }
        else {
            logwarn("no such module!");
        }
    }

    loginfo("initializing modules");

    for (auto pair : modules) {
        auto module = pair.module;
        auto mc = ctx.module(pair.found.position);

        if (!mc.open()) {
            logerror("error choosing module");
            return tl::unexpected<Error>(Error::General);
        }

        if (!module->initialize(mc, pool_)) {
            logerror("error initializing module");
            return tl::unexpected<Error>(Error::General);
        }
    }

    return modules;
}

}
