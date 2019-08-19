#include "module_factory.h"

#include "scanning.h"
#include "registry.h"

namespace fk {

FK_DECLARE_LOGGER("modules");

ModuleFactory::ModuleFactory(ModMux *mm, Pool *pool) : mm_(mm), pool_(pool) {
}

ModuleFactory::~ModuleFactory() {
}

bool ModuleFactory::create() {
    ModuleScanning scanning{ mm_ };
    auto module_headers = scanning.scan(pool());
    if (!module_headers) {
        logerror("error scanning modules");
        return false;
    }

    ModuleRegistry registry;
    auto resolved = registry.resolve(*module_headers, pool());
    if (!resolved) {
        logerror("error resolving modules");
        return false;
    }

    return true;
}

}
