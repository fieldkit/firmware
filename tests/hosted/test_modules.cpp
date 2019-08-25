#include <hal/hal.h>

#include "test_modules.h"

namespace fk {

static ModuleSensors fk_module_fake_empty_sensors = {
    .nsensors = 0,
    .sensors = nullptr,
};

class FakeModuleEmpty : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
    }

    ModuleSensors const *get_sensors(ModuleContext mc, Pool &pool) override {
        return &fk_module_fake_empty_sensors;
    }

    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override {
        return nullptr;
    }
};

static Module *fk_test_module_create_empty(Pool &pool) {
    return new(pool) FakeModuleEmpty();
}

ModuleMetadata const fk_test_module_fake_empty = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x03,
    .name = "fake-empty",
    .ctor = fk_test_module_create_empty,
};

static SensorMetadata const fk_module_fake_1_sensor_metas[] = {
    { .name = "sensor", .unitOfMeasure = "" },
};

static ModuleSensors fk_module_fake_1_sensors = {
    .nsensors = sizeof(fk_module_fake_1_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_fake_1_sensor_metas,
};

class FakeModule1 : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
    }

    ModuleSensors const *get_sensors(ModuleContext mc, Pool &pool) override {
        return &fk_module_fake_1_sensors;
    }

    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override {
        auto mr = new(pool) NModuleReadings<1>();
        mr->set(0, (float)fk_random_i32(20, 100));
        return mr;
    }
};

static Module *fk_test_module_create_1(Pool &pool) {
    return new(pool) FakeModule1();
}

ModuleMetadata const fk_test_module_fake_1 = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x02,
    .name = "fake-1",
    .ctor = fk_test_module_create_1,
};

static SensorMetadata const fk_module_fake_2_sensor_metas[] = {
    { .name = "sensor-0", .unitOfMeasure = "" },
    { .name = "sensor-1", .unitOfMeasure = "" },
    { .name = "sensor-2", .unitOfMeasure = "" },
    { .name = "sensor-3", .unitOfMeasure = "" },
    { .name = "sensor-4", .unitOfMeasure = "" },
    { .name = "sensor-5", .unitOfMeasure = "" },
    { .name = "sensor-6", .unitOfMeasure = "" },
    { .name = "sensor-7", .unitOfMeasure = "" },
    { .name = "sensor-8", .unitOfMeasure = "" },
    { .name = "sensor-9", .unitOfMeasure = "" },
};

static ModuleSensors fk_module_fake_2_sensors = {
    .nsensors = sizeof(fk_module_fake_2_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_fake_2_sensor_metas,
};

class FakeModule2 : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
    }

    ModuleSensors const *get_sensors(ModuleContext mc, Pool &pool) override {
        return &fk_module_fake_2_sensors;
    }

    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override {
        auto mr = new(pool) NModuleReadings<10>();
        for (size_t i = 0; i < mr->size(); i++) {
            mr->set(i, (float)fk_random_i32(20, 100));
        }
        return mr;
    }
};

static Module *fk_test_module_create_2(Pool &pool) {
    return new(pool) FakeModule2();
}

ModuleMetadata const fk_test_module_fake_2 = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x03,
    .name = "fake-2",
    .ctor = fk_test_module_create_2,
};

ModuleMetadata const fk_test_module_fake_random = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x01,
    .name = "fake-random",
    .ctor = fk_test_module_create_2,
};

ModuleMetadata const fk_test_module_fake_diagnostics = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_DIAGNOSTICS,
    .version = 0x01,
    .name = "fake-random",
    .ctor = fk_test_module_create_2,
};

}
