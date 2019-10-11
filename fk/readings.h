#pragma once

#include <fk-data-protocol.h>

#include "state.h"

namespace fk {

class Readings {
private:
    ModMux *mm_;
    fk_data_DataRecord record_;

public:
    Readings(ModMux *mm);

public:
    tl::expected<ModuleReadingsCollection, Error> take_readings(ScanningContext &ctx, ConstructedModulesCollection const &modules,
                                                                uint32_t meta_record, uint32_t reading_number, Pool &pool);

public:
    fk_data_DataRecord &record();

};


}
