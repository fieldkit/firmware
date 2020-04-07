#pragma once

#include "state.h"
#include "modules/module_factory.h"
#include "readings_taker.h"

#include "mocks_and_fakes.h"
#include "test_modules.h"

namespace fk {

class DataWriter {
private:
    TwoWireWrapper module_bus_{ "modules", nullptr };
    GlobalState gs_;
    DataMemory *memory_;

public:
    DataWriter(DataMemory *memory) : memory_(memory) {
    }

public:
    bool write(Pool &pool, int32_t number_of_modules) {
        Storage storage{ memory_, pool, false };
        if (!storage.begin()) {
            if (!storage.clear()) {
                return false;
            }
        }

        ScanningContext ctx{ get_modmux(), &gs_, module_bus_, pool };

        FoundModuleCollection found(pool);
        for (auto i = 0; i < number_of_modules; ++i) {
            found.emplace(FoundModule{ .position = 0xff,
                                       .header = {
                                           .manufacturer = FK_MODULES_MANUFACTURER,
                                           .kind = FK_MODULES_KIND_RANDOM,
                                           .version = 0x01,
                                       } });
        }
        StaticModuleScanning scanning(found);
        ModuleFactory module_factory;
        auto constructed_maybe = module_factory.rescan_and_initialize(ctx, scanning, pool);

        ReadingsTaker readings_taker{ storage, get_modmux(), false };
        return (bool)readings_taker.take(*constructed_maybe, ctx, pool);
    }
};

} // namespace fk
