#include "export_data_worker.h"
#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"
#include "utilities.h"
#include "clock.h"
#include "state_manager.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("expdata");

ExportDataWorker::ExportDataWorker() {
    info_ = { "CSV", 0.0f, true };
}

void ExportDataWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool };
    if (!storage.begin()) {
        return;
    }

    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
    auto path = pool.sprintf("/%s/data.csv", formatted.cstr());

    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto writing = sd->open(path, true, pool);
    if (writing == nullptr || !writing) {
        logerror("unable to open '%s'", path);
        return;
    }

    auto reading = storage.file(Storage::Data);

    if (!reading.seek_end()) {
        logerror("seek end failed");
        return;
    }

    auto total_bytes = reading.size();

    if (!reading.seek_beginning()) {
        logerror("seek beginning failed");
        return;
    }

    StandardPool loop_pool{ "decode" };

    auto bytes_read = 0u;
    auto nrecords = 0u;
    while (bytes_read < total_bytes) {
        auto record = fk_data_record_decoding_readings_new(loop_pool);
        auto record_read = reading.read(&record, fk_data_DataRecord_fields);
        if (record_read == 0) {
            loginfo("done");
            break;
        }

        info_.progress = (float)bytes_read / total_bytes;
        bytes_read += record_read;
        nrecords++;

        if (nrecords % 100 == 0) {
            auto sensor_groups_array = reinterpret_cast<pb_array_t*>(record.readings.sensorGroups.arg);
            auto sensor_groups = reinterpret_cast<fk_data_SensorGroup*>(sensor_groups_array->buffer);

            loginfo("reading: time = %" PRIu32 " meta = %" PRIu32 " groups = %zu",
                    (uint32_t)record.readings.time, record.readings.meta, sensor_groups_array->length);

            for (auto i = 0u; i < sensor_groups_array->length; ++i) {
                auto sensor_group = sensor_groups[i];
                auto sensor_values_array = reinterpret_cast<pb_array_t*>(sensor_group.readings.arg);
                // auto sensor_values = reinterpret_cast<fk_data_SensorGroup*>(sensor_values_array->buffer);
                loginfo("reading: sensors = %zu", sensor_values_array->length);
            }
        }

        loop_pool.clear();
    }

    if (!writing->close()) {
        logerror("error closing");
        return;
    }
}

} // namespace fk
