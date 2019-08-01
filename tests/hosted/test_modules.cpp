#include <hal/hal.h>

#include "test_modules.h"

namespace fk {

class FakeModule : public Module {
public:
    bool initialize(ModuleContext mc, Pool &pool) override {
        return true;
    }

    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override {
        auto mr = new(pool) ModuleReadings(1);
        mr->set(0, (float)fk_random_i32(20, 100));
        return mr;
    }
};

static Module *fk_test_module_create_1(Pool &pool) {
    return new(pool) FakeModule();
}

ModuleMetadata const fk_test_module_fake_1 = {
    .manufacturer = FK_MODULES_MANUFACTURER,
    .kind = FK_MODULES_KIND_RANDOM,
    .version = 0x01,
    .name = "fake-1",
    .ctor = fk_test_module_create_1,
};

}
