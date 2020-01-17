#include "modules/module_factory.h"

#include "modules/scanning.h"
#include "modules/registry.h"

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

optional<ConstructedModule> ModuleFactory::get(uint8_t bay) {
    for (auto &m : modules_) {
        if (m.found.position == bay) {
            return m;
        }
    }

    return nullopt;
}

bool ModuleFactory::changes(FoundModuleCollection &a, ConstructedModulesCollection &b) {
    if (a.size() != b.size()) {
        return true;
    }

    auto a_iter = a.begin();
    auto b_iter = b.begin();

    for (auto i = 0u; i < a.size(); ++i) {
        if (memcmp(&a_iter->header.id, &b_iter->found.header.id, sizeof(a_iter->header.id)) != 0) {
            return true;
        }

        ++a_iter;
        ++b_iter;
    }

    return false;
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::rescan_and_initialize(ScanningContext ctx, ModuleScanning &scanning, Pool &pool) {
    auto modules_maybe = rescan(scanning, pool);
    if (!modules_maybe) {
        return tl::unexpected<Error>(modules_maybe.error());
    }

    if (!initialize(ctx, pool)) {
        return tl::unexpected<Error>(Error::General);
    }

    return ConstructedModulesCollection(modules_);
}

bool ModuleFactory::initialize(ScanningContext &ctx, Pool &pool) {
    auto success = true;

    for (auto &pair : modules_) {
        auto module = pair.module;
        if (module != nullptr) {
            if (pair.status == ModuleStatus::Found) {
                auto mc = ctx.module(pair.found.position);
                if (!mc.open()) {
                    logerror("error opening module");
                    return false;
                }

                auto mr = module->initialize(mc, pool);
                if (!mr) {
                    success = false;
                }

                pair.status = mr.status;

                auto config = module->get_configuration(pool_);
                if (config.service_interval > 0) {
                    if (service_interval_ > 0) {
                        service_interval_ = std::min(config.service_interval, service_interval_);
                    } else {
                        service_interval_ = config.service_interval;
                    }
                }
            }
        }
    }

    return success;
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

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::resolve(FoundModuleCollection &module_headers, Pool &pool) {
    ConstructedModulesCollection modules{ pool };
    ModuleRegistry registry;
    for (auto &f : module_headers) {
        auto meta = registry.resolve(f.header);
        if (meta == nullptr) {
            modules.emplace(ConstructedModule{
                .found = f,
                .meta = nullptr,
                .module = nullptr,
                .status = ModuleStatus::Fatal,
            });

            logwarn("no such module!");
        }
        else {
            auto module = meta->ctor(pool);
            modules.emplace(ConstructedModule{
                .found = f,
                .meta = meta,
                .module = module,
                .status = ModuleStatus::Found,
            });
        }
    }

    return ConstructedModulesCollection(modules);
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::rescan(ModuleScanning &scanning, Pool &pool) {
    auto module_headers = scanning.scan(pool);
    if (!module_headers) {
        return tl::unexpected<Error>(module_headers.error());
    }

    clear();

    auto resolved_maybe = resolve(*module_headers, pool_);
    if (!resolved_maybe) {
        return tl::unexpected<Error>(resolved_maybe.error());
    }

    modules_.add(*resolved_maybe);

    return ConstructedModulesCollection(modules_);
}

void ModuleFactory::clear() {
    pool_.clear();
    modules_ = { pool_ };
}

}
