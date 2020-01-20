#include <hal/hal.h>

#include "test_modules.h"
#include "modules/shared/modules.h"

namespace fk {

static ModuleSensors fk_module_fake_empty_sensors = {
    .nsensors = 0,
    .sensors = nullptr,
};

class FakeModuleEmpty : public FakeModule {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override {
        return { ModuleStatus::Ok };
    }

    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleReturn service(ModuleContext mc, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleSensors const *get_sensors(Pool &pool) override {
        return &fk_module_fake_empty_sensors;
    }

    ModuleConfiguration get_configuration(Pool &pool) override {
        return { };
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
    .flags = 0,
    .ctor = fk_test_module_create_empty,
};

static SensorMetadata const fk_module_fake_1_sensor_metas[] = {
    { .name = "sensor", .unitOfMeasure = "", .flags = 0 },
};

static ModuleSensors fk_module_fake_1_sensors = {
    .nsensors = sizeof(fk_module_fake_1_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_fake_1_sensor_metas,
};

class FakeModule1 : public FakeModule {
private:

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override {
        return { ModuleStatus::Ok };
    }

    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleReturn service(ModuleContext mc, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleSensors const *get_sensors(Pool &pool) override {
        return &fk_module_fake_1_sensors;
    }

    ModuleConfiguration get_configuration(Pool &pool) override {
        return { };
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
    .flags = 0,
    .ctor = fk_test_module_create_1,
};

static SensorMetadata const fk_module_fake_2_sensor_metas[] = {
    { .name = "sensor-0", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-1", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-2", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-3", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-4", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-5", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-6", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-7", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-8", .unitOfMeasure = "", .flags = 0 },
    { .name = "sensor-9", .unitOfMeasure = "", .flags = 0 },
};

static ModuleSensors fk_module_fake_2_sensors = {
    .nsensors = sizeof(fk_module_fake_2_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_fake_2_sensor_metas,
};

class FakeModule2 : public FakeModule {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override {
        return { ModuleStatus::Ok };
    }

    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleReturn service(ModuleContext mc, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleSensors const *get_sensors(Pool &pool) override {
        return &fk_module_fake_2_sensors;
    }

    ModuleConfiguration get_configuration(Pool &pool) override {
        return { };
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
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_test_module_create_2,
};

ModuleMetadata const fk_test_module_fake_random = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x01,
    .name = "fake-random",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_test_module_create_2,
};

ModuleMetadata const fk_test_module_fake_diagnostics = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_DIAGNOSTICS,
    .version = 0x01,
    .name = "fake-random",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_test_module_create_2,
};

}
