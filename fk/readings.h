#pragma once

#include <fk-data-protocol.h>

#include "containers.h"

#include "state.h"
#include "modules/module_factory.h"

namespace fk {

struct ModuleMetaAndReadings {
    uint16_t position;
    fk_uuid_t *id;
    ModuleMetadata const *meta;
    ModuleSensors const *sensors;
    ModuleReadings const *readings;
};

using ModuleReadingsCollection = std::list<ModuleMetaAndReadings , pool_allocator<ModuleMetaAndReadings>>;

class Readings {
private:
    ModMux *mm_;
    fk_data_DataRecord record_;

public:
    Readings(ModMux *mm);

public:
    tl::expected<ModuleReadingsCollection, Error> take_readings(ModuleContext &mc, ConstructedModulesCollection const &modules, uint32_t reading_number, Pool &pool);

public:
    fk_data_DataRecord &record();

};


}
