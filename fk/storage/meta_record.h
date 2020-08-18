#pragma once

#include <loading.h>
#include <fk-data-protocol.h>

#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class MetaRecord {
private:
    fk_data_DataRecord record_;

public:
    void include_state(GlobalState const *gs, fkb_header_t const *fkb, Pool &pool);
    void include_modules(GlobalState const *gs, fkb_header_t const *fkb, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool);

public:
    fk_data_DataRecord &for_decoding(Pool &pool);
    fk_data_DataRecord const &record();

};

}
