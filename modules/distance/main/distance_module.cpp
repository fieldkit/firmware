#include "distance_module.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("distance");

DistanceModule::DistanceModule() : bridge_(get_board()->acquire_i2c_module()) {
}

ModuleReturn DistanceModule::initialize(ModuleContext mc, Pool &pool) {
    auto bus = get_board()->i2c_module();

    bus.end();
    bus.begin();

    // enumerate bus
    for (auto i = 0u; i < 128u; ++i) {
        auto rv = bus.write(i, nullptr, 0);
        if (I2C_CHECK(rv)) {
            loginfo("found 0x%x", i);
        }
    }

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReturn DistanceModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "unsupported");

    return { ModuleStatus::Fatal };
}

ModuleReturn DistanceModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static SensorMetadata const fk_module_distance_sensor_metas[] = {
    { .name = "distance_0",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_1",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_2",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "calibration", .unitOfMeasure = "mm", .flags = 0 },
};

static ModuleSensors fk_module_distance_sensors = {
    .nsensors = sizeof(fk_module_distance_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_distance_sensor_metas,
};

ModuleSensors const *DistanceModule::get_sensors(Pool &pool) {
    return &fk_module_distance_sensors;
}

ModuleConfiguration DistanceModule::get_configuration(Pool &pool) {
    return { "modules.distance" };
}

ModuleReadings *DistanceModule::take_readings(ReadingsContext mc, Pool &pool) {
    auto mr = new(pool) NModuleReadings<4>();
    auto nreadings = 0u;

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge");
        return nullptr;
    }

    for (auto i = 0; i < 5; ++i) {
        const char *line = nullptr;
        if (!line_reader_.read_line_sync(&line, 1000)) {
            logerror("error reading line");
        }

        if (line != nullptr) {
            if (line[0] == 'R' && line[1] != 0) {
                auto value = atoi(line + 1);
                mr->set(nreadings++, (float)value);
                logdebug("[%d] line: %d", nreadings, value);
                if (nreadings == 3) {
                    break;
                }
            }
            else {
                logwarn("[%d] line: '%s'", nreadings, line);
            }
        }
    }

    if (nreadings < 3) {
        logerror("unable to gather enough readings (%d)", nreadings);
        return nullptr;
    }

    // Calibration value
    mr->set(nreadings++, (float)0.0f);

    return mr;
}

} // namespace fk
