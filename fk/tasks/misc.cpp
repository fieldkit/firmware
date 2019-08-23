#include "tasks/tasks.h"
#include "config.h"
#include "storage/storage.h"
#include "hal/metal/metal.h"

#include "hal/hal.h"
#include "readings_taker.h"

namespace fk {

FK_DECLARE_LOGGER("misc");

void task_handler_misc(void *params) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        storage.fsck();
    }
    else {
        FK_ASSERT(storage.clear());

        auto pool = MallocPool{ "readings", ModuleMemoryAreaSize };
        FoundModuleCollection found(pool);
        found.emplace_back(FoundModule{
            .position = 0xff,
            .header = {
                .manufacturer = FK_MODULES_MANUFACTURER,
                .kind = FK_MODULES_KIND_RANDOM,
                .version = 0x01,
            }
        });

        TwoWireWrapper module_bus{ "modules", nullptr };
        ModuleContext mc{ get_global_state_rw().get(), module_bus };
        StaticModuleScanning scanning(found);
        ReadingsTaker readings_taker{ scanning, storage, get_modmux() };
        FK_ASSERT(readings_taker.take(mc, pool));

        auto data = storage.file(Storage::Data);

        for (auto i = 0; i < 30000; ++i) {
            FK_ASSERT(readings_taker.append_readings(data, pool));
        }
    }
}

}
