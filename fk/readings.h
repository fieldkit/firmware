#pragma once

#include <fk-data-protocol.h>

#include "registry.h"
#include "state.h"
#include "containers.h"

namespace fk {

using ModuleReadingsCollection = std::list<ModuleReadings*, pool_allocator<ModuleReadings*>>;

class Readings {
private:
    ModMux *mm_;
    fk_data_DataRecord record_;

public:
    Readings(ModMux *mm);

public:
    nonstd::optional<ModuleReadingsCollection> take_readings(ModuleContext &mc, ConstructedModulesCollection const &modules, uint32_t reading_number, Pool &pool);

public:
    fk_data_DataRecord &record();

};


}
