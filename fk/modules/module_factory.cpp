#include "modules/module_factory.h"

#include "modules/scanning.h"
#include "modules/registry.h"

namespace fk {

FK_DECLARE_LOGGER("modules");

template<typename T>
class lazy_allocation {
private:
    uint8_t buffer_[sizeof(T)]{ };
    T *ptr_{ nullptr };

public:
    T *get() {
        if (ptr_ == nullptr) {
            ptr_ = new (buffer_) T();
        }
        return ptr_;
    }

};


static lazy_allocation<ModuleFactory> module_factory;

ModuleFactory &get_module_factory() {
    return *module_factory.get();
}

ModuleFactory::ModuleFactory() {
}

ModuleFactory::~ModuleFactory() {
}

optional<ConstructedModule*> ModuleFactory::get(ModulePosition position) {
    for (auto &m : modules_) {
        if (m.found.position == position) {
            return &m;
        }
    }

    return nullopt;
}

#if !defined(__SAMD51__)
void ModuleFactory::modules(ConstructedModulesCollection modules) {
    clear();
    modules_.add(modules);
}
#endif

ConstructedModulesCollection ModuleFactory::modules() {
    return ConstructedModulesCollection(modules_);
}

uint32_t ModuleFactory::service_interval() const {
    return service_interval_;
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

    for (auto &constructed : modules_) {
        auto mod = constructed.module;
        if (mod != nullptr) {
            if (constructed.status == ModuleStatus::Found) {
                auto mc = ctx.module(constructed.found.position, pool);
                auto position = constructed.found.position;
                if (!mc.open()) {
                    logerror("[%d] error opening module", position.integer());
                    return false;
                }

                auto mr = mod->initialize(mc, pool_);
                if (mr.status != ModuleStatus::Ok) {
                    logerror("[%d] error initializing module", position.integer());
                }

                auto config = mod->get_configuration(pool);
                if (config.service_interval > 0) {
                    if (service_interval_ > 0) {
                        service_interval_ = std::min(config.service_interval, service_interval_);
                    } else {
                        service_interval_ = config.service_interval;
                    }
                }
                constructed.configuration = config;
                constructed.status = mr.status;
            }
        }
    }

    sort_modules();

    return success;
}

void ModuleFactory::sort_modules() {
    modules_.sort([=](ConstructedModule constructed) -> ModuleOrder {
        return constructed.configuration.service_order;
    });
}

bool ModuleFactory::service(ScanningContext &ctx, Pool &pool) {
    return true;
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::resolve(FoundModuleCollection &module_headers, Pool &pool) {
    ConstructedModulesCollection modules{ pool };
    ModuleRegistry registry;
    for (auto &f : module_headers) {
        auto meta = registry.resolve(f.header);
        FK_ASSERT(meta != nullptr);
        auto module = meta->ctor(pool);
        modules.emplace(ConstructedModule{
            .found = f,
            .meta = meta,
            .module = module,
            .status = ModuleStatus::Found,
        });
    }

    return ConstructedModulesCollection(modules);
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::rescan(ModuleScanning &scanning, Pool &pool) {
    auto module_headers = scanning.scan(pool);
    if (!module_headers) {
        return tl::unexpected<Error>(module_headers.error());
    }

    if (!changes(*module_headers, modules_)) {
        return ConstructedModulesCollection(modules_);
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
