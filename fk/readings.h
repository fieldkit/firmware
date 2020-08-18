#pragma once

#include <fk-data-protocol.h>

#include "state.h"
#include "modules/bridge/contexts.h"

namespace fk {

class Readings {
private:
    ModMux *mm_;
    fk_data_DataRecord record_;

public:
    explicit Readings(ModMux *mm);

public:
    tl::expected<ModuleReadingsCollection, Error> take_readings(ScanningContext &ctx, ConstructedModulesCollection const &modules, Pool &pool);

public:
    fk_data_DataRecord const &record();

    void link(uint32_t meta_record, uint32_t reading_number);

    void bump_amplified_reading() {
        record_.readings.reading++;
    }

};


}
