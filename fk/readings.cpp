#include "readings.h"
#include "clock.h"
#include "records.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

Readings::Readings(ModMux *mm) : mm_(mm) {
}

tl::expected<ModuleReadingsCollection, Error> Readings::take_readings(
    ScanningContext &ctx, ConstructedModulesCollection const &modules,
    uint32_t meta_record, uint32_t reading_number, Pool &pool) {
    ModuleReadingsCollection all_readings{ pool };

    auto now = get_clock_now();
    auto gs = ctx.gs();

    record_ = fk_data_record_encoding_new();
    record_.readings.time = now;
    record_.readings.reading = reading_number;
    record_.readings.meta = meta_record;
    record_.readings.flags = fk_data_DownloadFlags_READING_FLAGS_NONE;
    record_.readings.location.time = gs->gps.time;
    record_.readings.location.fix = gs->gps.fix;
    record_.readings.location.longitude = gs->gps.longitude;
    record_.readings.location.latitude = gs->gps.latitude;
    record_.readings.location.altitude = gs->gps.altitude;
    record_.readings.location.satellites = gs->gps.satellites;
    record_.readings.location.hdop = gs->gps.hdop;

    if (modules.size() == 0) {
        return all_readings;
    }

    auto groups = pool.malloc<fk_data_SensorGroup>(modules.size());
    auto group_number = 0;

    bzero(groups, sizeof(fk_data_SensorGroup) * modules.size());

    for (auto pair : modules) {
        auto meta = pair.meta;
        auto module = pair.module;
        auto i = pair.found.position;

        if (module == nullptr) {
            continue;
        }

        auto mc = ctx.module(i);

        if (!mc.open()) {
            logerror("[%d] error choosing module", i);
            continue;
        }

        loginfo("'%s' mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32, meta->name, meta->manufacturer, meta->kind, meta->version);

        auto readings = module->take_readings(mc, pool);
        if (readings == nullptr) {
            logwarn("'%s' no readings", meta->name);
            continue;
        }

        loginfo("'%s' %zd readings", meta->name, readings->size());

        auto sensor_metas = module->get_sensors(pool);
        auto sensor_values = pool.malloc<fk_data_SensorAndValue>(readings->size());
        for (uint32_t i = 0; i < readings->size(); ++i) {
            auto value = readings->get(i);
            loginfo("[%2" PRIu32 "] '%s.%s' = %f", i, meta->name, sensor_metas->sensors[i].name, value);
            sensor_values[i] = { i, value };
        }

        auto readings_array = pool.malloc_with<pb_array_t>({
            .length = (size_t)readings->size(),
            .itemSize = sizeof(fk_data_SensorAndValue),
            .buffer = sensor_values,
            .fields = fk_data_SensorAndValue_fields,
        });

        auto &group = groups[group_number];
        group.module = i;
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = readings_array;
        group_number++;

        all_readings.emplace_back(ModuleMetaAndReadings{
            .position = pair.found.position,
            .id = (fk_uuid_t *)pool.copy(&pair.found.header.id, sizeof(pair.found.header.id)),
            .meta = meta,
            .sensors = module->get_sensors(pool),
            .readings = readings,
        });
    }

    auto sensor_groups_array = pool.malloc_with<pb_array_t>({
        .length = (size_t)modules.size(),
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = groups,
        .fields = fk_data_SensorGroup_fields,
    });

    record_.readings.sensorGroups.arg = sensor_groups_array;

    if (!mm_->choose_nothing()) {
        logerror("[-] error deselecting");
        return all_readings;
    }

    return all_readings;
}

fk_data_DataRecord &Readings::record() {
    return record_;
}

}
