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

tl::expected<ConstructedModule, Error> ModuleFactory::get(uint8_t bay) {
    for (auto &m : modules_) {
        if (m.found.position == bay) {
            return m;
        }
    }

    return tl::unexpected<Error>(Error::General);
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

    return ConstructedModulesCollection(modules_);
}

bool ModuleFactory::recreate(ScanningContext &ctx, FoundModuleCollection &module_headers) {
    loginfo("constructing modules");

    clear();

    ModuleRegistry registry;
    for (auto &f : module_headers) {
        auto meta = registry.resolve(f.header);
        if (meta != nullptr) {
            auto module = meta->ctor(pool_);
            modules_.emplace(ConstructedModule{
                .found = f,
                .meta = meta,
                .module = module,
                .status = ModuleStatus::Warning,
            });
        }
        else {
            logwarn("no such module!");

            modules_.emplace(ConstructedModule{
                .found = f,
                .meta = nullptr,
                .module = nullptr,
                .status = ModuleStatus::Fatal,
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

        auto config = module->get_configuration(pool_);
        if (config.service_interval > 0) {
            if (service_interval_ > 0) {
                service_interval_ = std::min(config.service_interval, service_interval_);
            }
            else {
                service_interval_ = config.service_interval;
            }
        }

        auto mr = module->initialize(mc, pool_);
        pair.status = mr.status;
    }

    return true;
}

bool ModuleFactory::service(ScanningContext &ctx, Pool &pool) {
    auto success = true;

    for (auto &pair : modules_) {
        auto module = pair.module;
        if (module != nullptr) {
            auto config = module->get_configuration(pool);
            if (config.service_interval > 0) {
                auto mc = ctx.module(pair.found.position);

                if (!mc.open()) {
                    logerror("error opening module");
                    return false;
                }

                if (!module->service(mc, pool)) {
                    success = false;
                }
            }
        }
    }

    return success;
}

void ModuleFactory::clear() {
    pool_.clear();
    modules_ = { pool_ };
}

}
