#pragma once

#include <fk-data-protocol.h>
#include <loading.h>

#include "state.h"

namespace fk {

class MetaRecord {
private:
    Pool *pool_{ nullptr };
    fk_data_DataRecord *record_{ nullptr };

public:
    MetaRecord(Pool &pool);

public:
    bool include_state(GlobalState const *gs, fkb_header_t const *fkb, Pool &pool);
    bool include_modules(GlobalState const *gs, fkb_header_t const *fkb, Pool &pool);

public:
    fk_data_DataRecord *for_decoding();
    fk_data_DataRecord *record();
};

} // namespace fk
