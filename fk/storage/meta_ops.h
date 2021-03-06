#pragma once

#include <loading.h>

#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_record.h"
#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class MetaOps {
private:
    Storage &storage_;

public:
    explicit MetaOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, Pool &pool);
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, fkb_header_t const *fkb_header, Pool &pool);
    tl::expected<uint32_t, Error> write_modules(GlobalState *gs, fkb_header_t const *fkb_header, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool);

};

}
