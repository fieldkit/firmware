#include <fk-data-protocol.h>

#include "tasks/tasks.h"
#include "hal/hal.h"

#include "readings_taker.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

// Quick code to write a module EEPROM header for development purposes.
// Eventually we should have a secret menu or something for doing this kind
// of thing.
static bool configure_module(uint8_t position, ModuleHeader &header) __attribute__((unused));
static bool configure_weather(uint8_t position) __attribute__((unused));
static bool configure_water(uint8_t position) __attribute__((unused));

void task_handler_readings(void *params) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    auto started = fk_uptime();
    auto pool = MallocPool{ "readings", ModuleMemoryAreaSize };
    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();
    ModuleContext mc{ gs.get(), module_bus };
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

    if (false) {
        FK_ASSERT(configure_water(6));
        FK_ASSERT(configure_weather(4));
    }

    ReadingsTaker readings_taker{ scanning, storage, get_modmux(), false };
    FK_ASSERT(readings_taker.take(mc, pool));
    memory.log_statistics();

    loginfo("done (pool = %d/%d bytes) (%" PRIu32 "ms)", pool.used(), pool.size(), fk_uptime() - started);
}

static bool configure_module(uint8_t position, ModuleHeader &header) {
    fk_uuid_generate(&header.id);

    ModuleScanning scanning{ get_modmux() };
    if (!scanning.configure(position, header)) {
        logerror("[%d] unable to configure module", position);
        return false;
    }

    return true;
}

static bool configure_weather(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WEATHER,
        .version = 0x01,
        .id = { 0 },
    };

    if (!configure_module(position, header)) {
        return false;
    }

    loginfo("[%d] configured weather", position);

    return true;
}

static bool configure_water(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER,
        .version = 0x01,
        .id = { 0 },
    };

    if (!configure_module(position, header)) {
        return false;
    }

    loginfo("[%d] configured water", position);

    return true;
}

}
