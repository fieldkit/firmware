#include "tests.h"

#include "modules/module_factory.h"
#include "mocks_and_fakes.h"
#include "state.h"

using testing::Return;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::_;
using namespace fk;

class ModuleFactorySuite : public ::testing::Test {
protected:
    MallocPool pool_{ "tests", 1024 * 100 };
};

TEST_F(ModuleFactorySuite, WithNoModules) {
    FoundModuleCollection found(pool_);

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(found));

    ModuleFactory module_factory;
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ASSERT_TRUE(module_factory.create(scanning, ctx, pool_));
}

TEST_F(ModuleFactorySuite, BasicSingleModule) {
    FoundModuleCollection found(pool_);
    found.emplace_back(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x01,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(found));

    ModuleFactory module_factory;
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ASSERT_TRUE(module_factory.create(scanning, ctx, pool_));
}
