#pragma once

#include "modules/bridge/modules_bridge.h"

namespace fk {

extern ModuleMetadata const fk_test_module_fake_random;
extern ModuleMetadata const fk_test_module_fake_diagnostics;
extern ModuleMetadata const fk_test_module_fake_1;
extern ModuleMetadata const fk_test_module_fake_2;
extern ModuleMetadata const fk_test_module_fake_empty;

class FakeModule : public Module {
};

}
