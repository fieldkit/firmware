#include "tests.h"

#include "module_factory.h"

using namespace fk;

class ModuleFactorySuite : public ::testing::Test {
protected:
    MallocPool pool_{ "tests", 1024 * 100 };
};

TEST_F(ModuleFactorySuite, BasicScan) {
    ModuleFactory module_factory(get_modmux(), &pool_);
    FK_ASSERT(module_factory.create());
}
