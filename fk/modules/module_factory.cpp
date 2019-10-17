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

    if ((*module_headers).size() != modules_.size()) {
        if (!recreate(ctx, *module_headers)) {
            return tl::unexpected<Error>(Error::General);
        }
    }
    else {
        logdebug("keep modules");
    }

    ConstructedModulesCollection constructed{ pool };
    constructed = modules_;
    return constructed;
}

bool ModuleFactory::recreate(ScanningContext &ctx, FoundModuleCollection module_headers) {
    loginfo("constructing modules");

    clear();

    ModuleRegistry registry;
    for (auto &f : module_headers) {
        auto meta = registry.resolve(f.header);
        if (meta != nullptr) {
            auto module = meta->ctor(pool_);
            modules_.emplace_back(ConstructedModule{
                .found = f,
                .meta = meta,
                .module = module,
                .initialized = false,
            });
        }
        else {
            logwarn("no such module!");

            modules_.emplace_back(ConstructedModule{
                .found = f,
                .meta = nullptr,
                .module = nullptr,
                .initialized = false,
            });
        }
    }

    for (auto &pair : modules_) {
        auto module = pair.module;
        if (module == nullptr) {
            continue;
        }

        auto mc = ctx.module(pair.found.position);

        if (!mc.open()) {
            logerror("error opening module");
            return false;
        }

        if (!module->initialize(mc, pool_)) {
            logerror("error initializing module");
            pair.initialized = false;
        }
        else {
            pair.initialized = true;
        }
    }

    loginfo("done (pool = %zd/%zd bytes)", pool_.used(), pool_.size());

    return true;
}

void ModuleFactory::clear() {
    modules_.clear();
    pool_.clear();
}

}
