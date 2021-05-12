#include "readings.h"
#include "clock.h"
#include "records.h"
#include "platform.h"
#include "modules/bridge/modules.h"
#include "modules/enable_module_power.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

Readings::Readings(ModMux *mm, Pool &pool) : mm_(mm) {
    record_ = pool.malloc<fk_data_DataRecord>();
}

tl::expected<ModuleReadingsCollection, Error> Readings::take_readings(ScanningContext &ctx, ConstructedModulesCollection const &modules, Pool &pool) {
    ModuleReadingsCollection all_readings{ pool };

    auto now = get_clock_now();
    auto gps = ctx.gps();

    fk_data_record_encoding_new(record_);
    record_->has_readings = true;
    record_->readings.time = now;
    // These get set via Readings::link.
    record_->readings.reading = 0;
    record_->readings.meta = 0;
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

    if (modules.size() == 0) {
        return std::move(all_readings);
    }

    auto groups = pool.malloc<fk_data_SensorGroup>(modules.size());
    auto group_number = 0;

    bzero(groups, sizeof(fk_data_SensorGroup) * modules.size());

    // Initialize an empty sensor group at each module position here,
    // so that if we `continue` in the loop after they're
    // initialized. This isn't ideal, continue just sucks.
    auto empty_readings_array = pool.malloc<pb_array_t>();
    empty_readings_array->length = 0;
    empty_readings_array->itemSize = sizeof(fk_data_SensorAndValue);
    empty_readings_array->buffer = nullptr;
    empty_readings_array->fields = fk_data_SensorAndValue_fields;

    for (auto i = 0u; i < modules.size(); ++i) {
        auto &group = groups[i];
        group.module = i;
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = empty_readings_array;
    }

    for (auto pair : modules) {
        auto meta = pair.meta;
        auto module = pair.module;
        auto position = pair.found.position;

        auto sensor_metas = module->get_sensors(pool);

        auto adding = ModuleMetaAndReadings{
            .position = pair.found.position,
            .id = (fk_uuid_t *)pool.copy(&pair.found.header.id, sizeof(pair.found.header.id)),
            .meta = meta,
            .sensors = sensor_metas,
            .readings = nullptr,
            .configuration = pair.configuration,
        };

        if (module == nullptr) {
            logwarn("[%d] ignore unknown module", position.integer());
            group_number++;
            all_readings.emplace(adding);
            continue;
        }

        EnableModulePower module_power{ true, pair.configuration.power, pair.found.position };
        if (!module_power.enable()) {
            logerror("[%d] error powering module", position.integer());
            group_number++;
            all_readings.emplace(adding);
            continue;
        }
        if (module_power.was_enabled()) {
            logdebug("wake delay: %" PRIu32 "ms", pair.configuration.wake_delay);
            fk_delay(pair.configuration.wake_delay);
        }

        auto mc = ctx.readings(position, all_readings, pool);
        if (!mc.open()) {
            logerror("[%d] error choosing module", position.integer());
            group_number++;
            all_readings.emplace(adding);
            continue;
        }

        loginfo("'%s' mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32, pair.configuration.display_name_key, meta->manufacturer, meta->kind, meta->version);

        // TODO Cache this? POWER
        auto module_configuration = module->get_configuration(pool);
        adding.configuration = module_configuration;
        if (module_configuration.message != nullptr) {
            loginfo("'%s' config ok (%zu bytes)", meta->name, module_configuration.message->size);
        }
        else {
            loginfo("'%s' config ok", meta->name);
        }

        auto readings = module->take_readings(mc, pool);
        if (readings == nullptr || readings->size() == 0) {
            logwarn("'%s' no readings", meta->name);
            group_number++;
            all_readings.emplace(adding);
            continue;
        }

        loginfo("'%s' %zd readings", meta->name, readings->size());

        auto sensor_values = pool.malloc<fk_data_SensorAndValue>(readings->size());
        for (auto sensor_index = 0u; sensor_index < readings->size(); ++sensor_index) {
            auto reading = readings->get(sensor_index);
            loginfo("bay[%d] sensor[%2d] '%s.%s' = %f (%f)", position.integer(), sensor_index, meta->name,
                    sensor_metas->sensors[sensor_index].name, reading.calibrated, reading.uncalibrated);
            sensor_values[sensor_index] = fk_data_SensorAndValue_init_default;
            sensor_values[sensor_index].sensor = sensor_index;
            sensor_values[sensor_index].value = reading.calibrated;
            sensor_values[sensor_index].uncalibrated = reading.uncalibrated;
        }

        auto readings_array = pool.malloc<pb_array_t>();
        readings_array->length = (size_t)readings->size();;
        readings_array->itemSize = sizeof(fk_data_SensorAndValue);
        readings_array->buffer = sensor_values;
        readings_array->fields = fk_data_SensorAndValue_fields;

        auto &group = groups[group_number];
        group.module = position.integer();
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = readings_array;
        group_number++;

        adding.readings = readings;
        all_readings.emplace(adding);
    }

    auto sensor_groups_array = pool.malloc<pb_array_t>();
    sensor_groups_array->length = (size_t)modules.size();
    sensor_groups_array->itemSize = sizeof(fk_data_SensorGroup);
    sensor_groups_array->buffer = groups;
    sensor_groups_array->fields = fk_data_SensorGroup_fields;

    record_->readings.sensorGroups.arg = sensor_groups_array;

    if (!mm_->choose_nothing()) {
        logerror("[-] error deselecting");
        return std::move(all_readings);
    }

    return std::move(all_readings);
}

void Readings::meta_record(uint32_t meta_record) {
    record_->readings.meta = meta_record;
}

void Readings::record_number(uint32_t record_number) {
    record_->readings.reading = record_number;
}

fk_data_DataRecord &Readings::record() {
    return *record_;
}

} // namespace fk
