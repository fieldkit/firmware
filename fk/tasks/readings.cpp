#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

#include <fk-data-protocol.h>

namespace fk {

FK_DECLARE_LOGGER("readings");

void task_handler_readings(void *params) {
    auto started = fk_uptime();

    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

    if (!storage.begin()) {
        logwarn("error opening storage, wiping...");
        if (!storage.clear()) {
            logerror("wiping storage failed!");
            return;
        }
    }

    auto now = get_clock()->now().unix_time();

    auto file = storage.file(0);

    fk_data_SensorAndValue readings[] = {
        { 0, (float)fk_random_i32(20, 100) },
        { 1, (float)fk_random_i32(20, 100) },
        { 2, (float)fk_random_i32(20, 100) },
        { 3, (float)fk_random_i32(20, 100) },
        { 4, (float)fk_random_i32(20, 100) },
        { 5, (float)fk_random_i32(20, 100) },
        { 6, (float)fk_random_i32(20, 100) },
        { 7, (float)fk_random_i32(20, 100) },
        { 8, (float)fk_random_i32(20, 100) },
        { 9, (float)fk_random_i32(20, 100) },
    };

    pb_array_t readings_array = {
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.time = now;
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.location.fix = 0;
    record.readings.location.time = now;
    record.readings.location.longitude = -118.2709223;
    record.readings.location.latitude = 34.0318047;
    record.readings.location.altitude = 100.0f;
    record.readings.readings.funcs.encode = pb_encode_array;
    record.readings.readings.arg = &readings_array;

    auto bytes_wrote = file.write(&record, fk_data_DataRecord_fields);
    if (bytes_wrote == 0) {
        logerror("error saving readings");
    }

    loginfo("wrote %d bytes (%d bytes) (0x%06x) (%dms)", bytes_wrote, file.size(), file.tail(), fk_uptime() - started);

    memory.log_statistics();

    storage.fsck();
}

}
