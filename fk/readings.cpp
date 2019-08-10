#include "readings.h"
#include "clock.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

Readings::Readings(ModMux *mm, GlobalState const *gs) : mm_(mm), gs_(gs) {
}

bool Readings::take_readings(ResolvedModules const &modules, uint32_t reading_number, Pool &pool) {
    auto now = get_clock_now();

    record_ = fk_data_DataRecord_init_default;
    record_.readings.time = now;
    record_.readings.reading = reading_number;
    record_.readings.flags = 0;
    record_.readings.location.time = now;
    record_.readings.location.fix = gs_->gps.fix;
    record_.readings.location.longitude = gs_->gps.longitude;
    record_.readings.location.latitude = gs_->gps.latitude;
    record_.readings.location.altitude = gs_->gps.altitude;

    if (modules.size() == 0) {
        return true;
    }

    auto groups = pool.malloc<fk_data_SensorGroup>(modules.size());
    auto group_number = 0;

    bzero(groups, sizeof(fk_data_SensorGroup) * modules.size());

    auto module_bus = get_board()->i2c_module();
    ModuleContext mc{ gs_, module_bus };
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        auto meta = modules.get(i);
        if (meta == nullptr) {
            continue;
        }

        if (!mm_->choose(i)) {
            logerror("error choosing module");
            continue;
        }

        loginfo("'%s' mk=%02" PRIx32 "%02" PRIx32 " version=%" PRIu32, meta->name, meta->manufacturer, meta->kind, meta->version);

        auto module = meta->ctor(pool);
        auto readings = module->take_readings(mc.module(i), pool);
        if (readings == nullptr) {
            logwarn("'%s' no readings", meta->name);
            continue;
        }

        auto sensor_values = pool.malloc<fk_data_SensorAndValue>(readings->size());

        for (uint32_t i = 0; i < readings->size(); ++i) {
            sensor_values[i] = { i, readings->get(i) };
        }

        auto readings_array = pool.malloc<pb_array_t>();
        *readings_array = {
            .length = (size_t)readings->size(),
            .itemSize = sizeof(fk_data_SensorAndValue),
            .buffer = sensor_values,
            .fields = fk_data_SensorAndValue_fields,
        };

        auto &group = groups[group_number];
        group.module = i;
        group.readings.funcs.encode = pb_encode_array;
        group.readings.arg = readings_array;
        group_number++;

        loginfo("'%s' %zd readings", meta->name, readings->size());
    }

    auto sensor_groups_array = pool.malloc<pb_array_t>();

    *sensor_groups_array = {
        .length = (size_t)modules.size(),
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = groups,
        .fields = fk_data_SensorGroup_fields,
    };

    record_.readings.sensorGroups.funcs.encode = pb_encode_array;
    record_.readings.sensorGroups.arg = sensor_groups_array;

    return true;
}

fk_data_DataRecord &Readings::record() {
    return record_;
}

}
