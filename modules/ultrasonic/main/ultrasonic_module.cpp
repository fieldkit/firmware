#include "ultrasonic_module.h"
#include "platform.h"

using namespace fk;

FK_DECLARE_LOGGER("ultrasonic");

UltrasonicModule::UltrasonicModule() : bridge_(get_board()->acquire_i2c_module()) {
}

bool UltrasonicModule::initialize(ModuleContext mc, fk::Pool &pool) {
    loginfo("hello");

    auto bus = get_board()->i2c_module();

    for (auto i = 0u; i < 128u; ++i) {
        auto rv = bus.write(i, nullptr, 0);
        if (I2C_CHECK(rv)) {
            loginfo("found 0x%x", i);
        }
    }

    if (!bridge_.begin(9600)) {
        logerror("initializing bridge");
        FK_ASSERT(0);
        return false;
    }

    FK_ASSERT(0);

    return true;
}

static SensorMetadata const fk_module_ultrasonic_sensor_metas[] = {
    { .name = "calibration", .unitOfMeasure = "cm", .flags = 0 },
    { .name = "distance",    .unitOfMeasure = "cm", .flags = 0 },
};

static ModuleSensors fk_module_ultrasonic_sensors = {
    .nsensors = sizeof(fk_module_ultrasonic_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_ultrasonic_sensor_metas,
};

ModuleSensors const *UltrasonicModule::get_sensors(Pool &pool) {
    return &fk_module_ultrasonic_sensors;
}

ModuleReadings *UltrasonicModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    const char *line = nullptr;
    if (!line_reader_.read_line_sync(&line, 1000)) {
        logerror("error reading line");
        return nullptr;
    }

    if (line != nullptr) {
        loginfo("line: %s", line);
    }

    return nullptr;
}
