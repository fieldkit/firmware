#pragma once

#include "storage/storage.h"
#include "modules/bridge/modules.h"
#include "readings.h"

namespace fk {

class ModMux;

class ReadingsTaker {
private:
    ModuleScanning &scanning_;
    Storage &storage_;
    Readings readings_;
    ModMux *mm_;
    bool read_only_;

public:
    ReadingsTaker(ModuleScanning &scanning, Storage &storage, ModMux *mm, bool read_only);

public:
    tl::expected<ModuleReadingsCollection, Error> take(ScanningContext &mc, Pool &pool);
    bool append_configuration(ScanningContext &ctx, ConstructedModulesCollection &modules, File &file, Pool &pool);
    bool append_readings(File &file, Pool &pool);

private:
    bool verify_reading_record(File &file, Pool &pool);

};

}
