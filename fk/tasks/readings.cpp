#include "tasks/tasks.h"

#include "hal/hal.h"

#include "clock.h"
#include "storage.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

void task_handler_readings(void *params) {
    auto memory = MemoryFactory::get_data_memory();
    Storage storage{ memory };

    if (!storage.begin()) {
        return;
    }

    auto now = get_clock()->now().unixtime();

    auto file = storage.file(0);

    fk_data_SensorAndValue readings[] = {
        { 0, (float)random(20, 100) },
        { 1, (float)random(20, 100) },
        { 2, (float)random(20, 100) },
        { 3, (float)random(20, 100) },
        { 4, (float)random(20, 100) },
        { 5, (float)random(20, 100) },
        { 6, (float)random(20, 100) },
        { 7, (float)random(20, 100) },
        { 8, (float)random(20, 100) },
        { 9, (float)random(20, 100) },
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

    loginfo("wrote %d bytes (%d bytes)", bytes_wrote, file.size());
}

}
