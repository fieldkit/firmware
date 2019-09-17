#pragma once

#include "storage/storage.h"
#include "storage/signed_log.h"
#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class MetaOps {
private:
    Storage &storage_;

public:
    MetaOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_state(GlobalState const *gs, Pool &pool);
    tl::expected<uint32_t, Error> write_modules(GlobalState const *gs, ConstructedModulesCollection &modules, Pool &pool);

};

}
