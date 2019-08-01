#include "random_module.h"

using namespace fk;

bool RandomModule::initialize(ModuleContext mc) {
    return true;
}

ModuleReadings *RandomModule::take_readings(ModuleContext mc) {
    auto mr = new ModuleReadings(10);

    for (auto i = 0; i < mr->size(); i++) {
        mr->set(i, (float)fk_random_i32(20, 100));
    }

    return mr;
}
