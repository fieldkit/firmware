#include "platform.h"
#include "storage/data_record.h"
#include "records.h"
#include "state.h"
#include "hal/clock.h"

namespace fk {

FK_DECLARE_LOGGER("data-record");

DataRecord::DataRecord(Pool &pool) {
    record_ = pool.malloc<fk_data_DataRecord>();
}

void DataRecord::include_readings(GlobalState const *gs, fkb_header_t const *fkb_header, uint32_t meta_record, Pool &pool) {
    auto now = get_clock_now();
    auto gps = gs->location(pool);

    fk_data_record_encoding_new(record_);

    record_->has_readings = true;
    record_->readings.time = now;
    // File ops fills this in.
    record_->readings.reading = 0;
    record_->readings.meta = meta_record;
    record_->readings.uptime = fk_uptime();
    record_->readings.flags = fk_data_DownloadFlags_READING_FLAGS_NONE;
    record_->readings.has_location = true;
    record_->readings.location.time = gps->time;
    record_->readings.location.fix = gps->fix;
    record_->readings.location.longitude = gps->longitude;
    record_->readings.location.latitude = gps->latitude;
    record_->readings.location.altitude = gps->altitude;
    record_->readings.location.satellites = gps->satellites;
    record_->readings.location.hdop = gps->hdop;

    if (gs->dynamic.attached() != nullptr) {
        auto attached = gs->dynamic.attached();
        auto nmodules = attached->modules().size();
        auto groups = pool.malloc<fk_data_SensorGroup>(nmodules);

        bzero(groups, sizeof(fk_data_SensorGroup) * nmodules);

        // Initialize an empty sensor group at each module position here,
        // so that if we `continue` in the loop after they're
        // initialized. This isn't ideal, continue just sucks.
        auto empty_readings_array = pool.malloc<pb_array_t>();
        empty_readings_array->length = 0;
        empty_readings_array->allocated = 0;
        empty_readings_array->item_size = sizeof(fk_data_SensorAndValue);
        empty_readings_array->buffer = nullptr;
        empty_readings_array->fields = fk_data_SensorAndValue_fields;

        for (auto i = 0u; i < nmodules; ++i) {
            auto &group = groups[i];
            group.module = i;
            group.readings.funcs.encode = pb_encode_array;
            group.readings.arg = empty_readings_array;
        }

        auto group_index = 0u;

        for (auto &attached_module : attached->modules()) {
            auto position = attached_module.position();
            auto meta = attached_module.meta();

            auto &sensors = attached_module.sensors();

            auto sensor_values = sensors.size() > 0 ? pool.malloc<fk_data_SensorAndValue>(sensors.size()) : nullptr;
            for (auto &sensor : sensors) {
                auto reading = sensor.reading();
                auto sensor_index = sensor.index();

                logverbose("[%d] sensor[%2d] name='%s.%s' calibrated=%f uncalibrated=%f", position.integer(), sensor_index, meta->name,
                           sensor.name(), reading.calibrated, reading.uncalibrated);
                sensor_values[sensor_index] = fk_data_SensorAndValue_init_default;
                sensor_values[sensor_index].sensor = sensor.index();
                sensor_values[sensor_index].value = reading.calibrated;
                sensor_values[sensor_index].uncalibrated = reading.uncalibrated;
            }

            auto &group = groups[group_index];
            group.module = position.integer();

            if (sensors.size() > 0) {
                auto readings_array = pool.malloc<pb_array_t>();
                readings_array->length = sensors.size();
                readings_array->allocated = sensors.size();
                readings_array->item_size = sizeof(fk_data_SensorAndValue);
                readings_array->buffer = sensor_values;
                readings_array->fields = fk_data_SensorAndValue_fields;

                group.readings.funcs.encode = pb_encode_array;
                group.readings.arg = readings_array;
            } else {
                group.readings.funcs.encode = pb_encode_array;
                group.readings.arg = nullptr;
            }

            group_index++;
        }

        auto sensor_groups_array = pool.malloc<pb_array_t>();
        sensor_groups_array->length = nmodules;
        sensor_groups_array->allocated = nmodules;
        sensor_groups_array->item_size = sizeof(fk_data_SensorGroup);
        sensor_groups_array->buffer = groups;
        sensor_groups_array->fields = fk_data_SensorGroup_fields;

        record_->readings.sensorGroups.arg = sensor_groups_array;
    }
}

fk_data_DataRecord &DataRecord::for_decoding(Pool &pool) {
    if (record_ == nullptr) {
        record_ = pool.malloc<fk_data_DataRecord>();
    }
    fk_data_record_decoding_new(record_, pool);
    return *record_;
}

fk_data_DataRecord &DataRecord::record() {
    FK_ASSERT(record_ != nullptr);
    return *record_;
}

} // namespace fk
