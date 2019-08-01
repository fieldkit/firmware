#include "readings.h"
#include "clock.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

Readings::Readings(GlobalState const *gs) : gs_(gs) {
}

bool Readings::take_readings(ResolvedModules const &modules, uint32_t reading_number, Pool &pool) {
    if (modules.size() == 0) {
        return true;
    }

    auto now = get_clock_now();
    auto groups = (fk_data_SensorGroup *)pool.malloc(sizeof(fk_data_SensorGroup) * modules.size());
    auto group_number = 0;

    ModuleContext mc;
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        auto meta = modules.get(i);
        if (meta == nullptr) {
            continue;
        }

        loginfo("calling '%s' mk=%02x%02x version=%d", meta->name,
                meta->manufacturer, meta->kind, meta->version);

        auto module = meta->ctor(pool);
        auto readings = module->take_readings(mc.module(i), pool);
        auto &group = groups[group_number];
        group.module = i;
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = readings;

        group_number++;
    }

    pb_array_t sensor_groups_array = {
        .length = (size_t)modules.size(),
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = groups,
        .fields = fk_data_SensorGroup_fields,
    };

    record_ = fk_data_DataRecord_init_default;
    record_.readings.time = now;
    record_.readings.reading = reading_number;
    record_.readings.flags = 0;
    record_.readings.location.time = now;
    record_.readings.location.fix = gs_->gps.fix;
    record_.readings.location.longitude = gs_->gps.longitude;
    record_.readings.location.latitude = gs_->gps.latitude;
    record_.readings.location.altitude = gs_->gps.altitude;
    record_.readings.sensorGroups.funcs.encode = pb_encode_array;
    record_.readings.sensorGroups.arg = &sensor_groups_array;

    return true;
}

fk_data_DataRecord &Readings::record() {
    return record_;
}

}
