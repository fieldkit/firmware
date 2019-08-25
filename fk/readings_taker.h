#pragma once

#include "scanning.h"
#include "storage/storage.h"
#include "modules_bridge.h"
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
    nonstd::optional<ModuleReadingsCollection> take(ModuleContext &mc, Pool &pool);
    bool append_readings(File &file, Pool &pool);
    bool append_configuration(ModuleContext &mc, ConstructedModulesCollection &modules, File &file, Pool &pool);
    bool initialize_modules(ModuleContext &mc, ConstructedModulesCollection &modules, ModMux *mm, Pool &pool);
    bool verify_reading_record(File &file, Pool &pool);

};

}
