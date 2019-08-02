#include <hal/hal.h>

#include "test_modules.h"

namespace fk {

class FakeModuleEmpty : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
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
    .version = 0x01,
    .name = "fake-empty",
    .ctor = fk_test_module_create_empty,
};

class FakeModule1 : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
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
    .version = 0x01,
    .name = "fake-1",
    .ctor = fk_test_module_create_1,
};

class FakeModule2 : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
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
    .version = 0x01,
    .name = "fake-2",
    .ctor = fk_test_module_create_2,
};

}
