#include <fk-data-protocol.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "readings_taker.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

// Quick code to write a module EEPROM header for development purposes.
// Eventually we should have a secret menu or something for doing this kind
// of thing.
static void configure_module(uint8_t position) __attribute__((unused));

void task_handler_readings(void *params) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto started = fk_uptime();
    auto pool = MallocPool{ "readings", ModuleMemoryAreaSize };
    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ModuleContext mc{ gs.get(), module_bus };

    FK_ASSERT(lock);

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };
    if (!storage.begin()) {
        logerror("error opening storage, wiping...");
        if (!storage.clear()) {
            logerror("wiping storage failed!");
            return;
        }
    }

    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ scanning, storage };
    readings_taker.take(mc, pool);

    memory.log_statistics();

    loginfo("done (pool = %d/%d bytes) (%" PRIu32 "ms)", pool.used(), pool.size(), fk_uptime() - started);
}

static void configure_module(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER,
        .version = 0x01,
        .reserved = { 0x00, 0x00, 0x00, 0x00 },
        .crc = 0x00,
    };
    ModuleScanning scanning{ get_modmux() };
    if (!scanning.configure(position, header)) {
        logerror("[%d] unable to configure module", position);
        return;
    }
}

}
