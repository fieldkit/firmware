#include <fk-data-protocol.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"
#include "state.h"

#include "scanning.h"
#include "registry.h"
#include "readings.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

void task_handler_readings(void *params) {
    auto pool = MallocPool{ "readings", 2048 };
    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto started = fk_uptime();

    ModuleScanning scanning{ get_modmux() };
    ModuleScan scan;

    if (!scanning.scan(scan)) {
        logerror("modules scan failed");
        return;
    }

    ModuleRegistry registry;
    ResolvedModules modules;

    if (!registry.resolve(scan, modules)) {
        logerror("modules resolve failed");
        return;
    }

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

    if (!storage.begin()) {
        logwarn("error opening storage, wiping...");
        if (!storage.clear()) {
            logerror("wiping storage failed!");
            return;
        }
    }

    auto file = storage.file(0);
    auto gs = get_global_state_ro();

    Readings readings{ gs.get() };
    if (!readings.take_readings(modules, file.record(), pool)) {
        logerror("error takign readings");
        return;
    }

    auto bytes_wrote = file.write(&readings.record(), fk_data_DataRecord_fields);
    if (bytes_wrote == 0) {
        logerror("error saving readings");
        return;
    }

    loginfo("wrote %d bytes (%d bytes) (0x%06x) (%dms)",
            bytes_wrote, file.size(), file.tail(), fk_uptime() - started);

    memory.log_statistics();

    storage.fsck();

    loginfo("done (pool = %d/%d bytes)", pool.used(), pool.size());
}

}
