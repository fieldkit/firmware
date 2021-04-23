#pragma once

#include <loading.h>
#include <fk-data-protocol.h>

#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class DataRecord {
private:
    fk_data_DataRecord *record_{ nullptr };

public:
    fk_data_DataRecord &for_decoding(Pool &pool);
    fk_data_DataRecord const &record();

};

} // namespace fk
