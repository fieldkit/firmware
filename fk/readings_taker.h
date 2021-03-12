#pragma once

#include "storage/storage.h"
#include "modules/bridge/modules.h"
#include "readings.h"

namespace fk {

class ModMux;

class ReadingsTaker {
private:
    Storage &storage_;
    Readings readings_;
    ModMux *mm_;
    bool read_only_;

public:
    ReadingsTaker(Storage &storage, ModMux *mm, bool read_only);

public:
    tl::expected<TakenReadings, Error> take(ConstructedModulesCollection &constructed_modules, ScanningContext &mc, Pool &pool);

private:
    tl::expected<uint32_t, Error> append_configuration(ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool);

    tl::expected<uint32_t, Error> append_readings(uint32_t meta_record, Pool &pool);

};

}
