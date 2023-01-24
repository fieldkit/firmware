#include "distance_module.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("distance");

constexpr uint32_t DistanceReadTimeout = 2000;

constexpr uint32_t DistanceMaximumReadings = 3;

DistanceModule::DistanceModule(Pool &pool) : bridge_(get_board()->acquire_i2c_module()) {
}

DistanceModule::~DistanceModule() {
}

ModuleReturn DistanceModule::initialize(ModuleContext mc, Pool &pool) {
    auto bus = get_board()->i2c_module();

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReturn DistanceModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported", pool);

    return { ModuleStatus::Fatal };
}

ModuleReturn DistanceModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static SensorMetadata const fk_module_distance_sensor_metas[] = {
    { .name = "distance_0", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_1", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_2", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = 0 },
    { .name = "calibration", .unitOfMeasure = "mm", .uncalibratedUnitOfMeasure = "mm", .flags = FK_MODULES_FLAG_INTERNAL },
};

static ModuleSensors fk_module_distance_sensors = {
    .nsensors = sizeof(fk_module_distance_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_distance_sensor_metas,
};

ModuleSensors const *DistanceModule::get_sensors(Pool &pool) {
    return &fk_module_distance_sensors;
}

ModuleConfiguration const DistanceModule::get_configuration(Pool &pool) {
    return { "modules.distance" };
}

ModuleReadings *DistanceModule::take_readings(ReadingsContext mc, Pool &pool) {
    auto mr = new (pool) NModuleReadings<4>();
    auto nreadings = 0u;

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge");
        return nullptr;
    }

    auto deadline = fk_uptime() + DistanceReadTimeout;
    while (fk_uptime() < deadline) {
        const char *line = nullptr;
        if (!line_reader_.read_line_sync(&line, 500)) {
            logerror("error reading line");
        }

        if (line != nullptr) {
            if (line[0] == 'R' && line[1] != 0) {
                auto value = atoi(line + 1);
                mr->set(nreadings++, SensorReading{ mc.now(), (float)value });
                logdebug("[%d] line: %d", nreadings, value);
                if (nreadings == DistanceMaximumReadings) {
                    break;
                }
            } else {
                logwarn("[%d] line: '%s'", nreadings, line);
            }
        }
    }

    auto elapsed = fk_uptime() - (deadline - DistanceReadTimeout);

    if (nreadings == 0) {
        logerror("no readings (%" PRIu32 "ms)", elapsed);
        return nullptr;
    }

    loginfo("done reading (%" PRIu32 "ms)", elapsed);

    // TODO Calibration value
    mr->set(nreadings++, SensorReading{ mc.now(), (float)0.0f });

    return mr;
}

} // namespace fk
