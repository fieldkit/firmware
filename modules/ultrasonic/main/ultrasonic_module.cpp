#include "ultrasonic_module.h"
#include "platform.h"

using namespace fk;

FK_DECLARE_LOGGER("ultrasonic");

UltrasonicModule::UltrasonicModule() : bridge_(get_board()->acquire_i2c_module()) {
}

bool UltrasonicModule::initialize(ModuleContext mc, fk::Pool &pool) {
    auto bus = get_board()->i2c_module();

    /*
    // enumerate bus
    for (auto i = 0u; i < 128u; ++i) {
        auto rv = bus.write(i, nullptr, 0);
        if (I2C_CHECK(rv)) {
            loginfo("found 0x%x", i);
        }
    }
    */

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge");
        return false;
    }

    return true;
}

static SensorMetadata const fk_module_ultrasonic_sensor_metas[] = {
    { .name = "distance_0",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_1",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "distance_2",  .unitOfMeasure = "mm", .flags = 0 },
    { .name = "calibration", .unitOfMeasure = "mm", .flags = 0 },
};

static ModuleSensors fk_module_ultrasonic_sensors = {
    .nsensors = sizeof(fk_module_ultrasonic_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_ultrasonic_sensor_metas,
};

ModuleSensors const *UltrasonicModule::get_sensors(Pool &pool) {
    return &fk_module_ultrasonic_sensors;
}

ModuleReadings *UltrasonicModule::take_readings(ModuleContext mc, fk::Pool &pool) {
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
            if (line[0] == 'R') {
                auto value = atof(line + 1);
                mr->set(nreadings++, value);
                logdebug("[%d] line: %s", nreadings, line);
                if (nreadings == 3) {
                    break;
                }
            }
            else {
                logwarn("[%d] line: %s", nreadings, line);
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
