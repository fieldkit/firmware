#include "module_factory.h"

#include "scanning.h"
#include "registry.h"

namespace fk {

FK_DECLARE_LOGGER("modules");

ModuleFactory::ModuleFactory(ModuleScanning &scanning, Pool *pool) : scanning_(&scanning), pool_(pool) {
}

ModuleFactory::~ModuleFactory() {
}

tl::expected<ConstructedModulesCollection, Error> ModuleFactory::create() {
    auto module_headers = scanning_->scan(pool());
    if (!module_headers) {
        logerror("error scanning modules");
        return tl::unexpected<Error>(module_headers.error());
    }

    ModuleRegistry registry;
    auto resolved = registry.resolve(*module_headers, pool());
    if (!resolved) {
        logerror("error resolving modules");
        return tl::unexpected<Error>(resolved.error());
    }

    return resolved;
}

}
