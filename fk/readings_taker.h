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
    bool verify_;

public:
    ReadingsTaker(Storage &storage, ModMux *mm, bool read_only, bool verify);

public:
    tl::expected<TakenReadings, Error> take(ConstructedModulesCollection &constructed_modules, ScanningContext &mc, Pool &pool);

    bool append_readings(File &file, Pool &pool);

private:
    tl::expected<uint32_t, Error> append_configuration(ConstructedModulesCollection &modules, Pool &pool);

    bool verify_reading_record(File &file, Pool &pool);

};

}
