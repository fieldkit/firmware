#include <fk-data-protocol.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "hal/metal/metal.h"
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
    auto lock = storage_mutex.acquire(UINT32_MAX);
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

    // Quick code to write a module EEPROM header for development purposes.
    // Eventually we should have a secret menu or something for doing this kind
    // of thing.
    if (false) {
        auto position = 6;
        ModuleHeader header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_WATER,
            .version = 0x01,
            .reserved = { 0x00, 0x00, 0x00, 0x00 },
            .crc = 0x00,
        };
        if (!scanning.configure(position, header)) {
            logerror("[%d] unable to configure module", position);
            return;
        }
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

    Readings readings{ get_modmux(), gs.get() };
    if (!readings.take_readings(modules, file.record(), pool)) {
        logerror("error taking readings");
        return;
    }

    for (auto i = 0; i < FK_INFLATE_WRITES_READINGS; ++i) {
        auto bytes_wrote = file.write(&readings.record(), fk_data_DataRecord_fields);
        if (bytes_wrote == 0) {
            logerror("error saving readings");
            return;
        }

        loginfo("wrote %d bytes (#%" PRIu32 ") (%" PRIu32 " bytes) (" PRADDRESS ") (%" PRIu32 "ms)",
                bytes_wrote, file.record() - 1, file.size(), file.tail(),
                fk_uptime() - started);
    }

    memory.log_statistics();

    loginfo("done (pool = %d/%d bytes)", pool.used(), pool.size());
}

}
