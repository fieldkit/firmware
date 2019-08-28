#include "tasks/tasks.h"
#include "hal/hal.h"

#include "storage/storage.h"
#include "readings_taker.h"
#include "state_ref.h"

#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("misc");

static bool fake_data_enabled = false;
static bool fake_data_inserted = true;

static void periodic_fsck() {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory() };
    if ((!fake_data_enabled || fake_data_inserted) && storage.begin()) {
        storage.fsck();
    }
    else if (fake_data_enabled) {
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
        ReadingsTaker readings_taker{ scanning, storage, get_modmux(), false };
        FK_ASSERT(readings_taker.take(mc, pool));

        auto data = storage.file(Storage::Data);

        FK_ASSERT(data.seek_end());

        for (size_t i = 0; i < 1000 / fk_config().readings.amplification; ++i) {
            FK_ASSERT(readings_taker.append_readings(data, pool));
        }

        fake_data_inserted = true;
    }
}

void task_handler_misc(void *params) {
    if (fk_config().debug.periodic_fsck) {
        periodic_fsck();
    }
}

}
