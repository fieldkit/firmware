#include "tests.h"

#include "module_factory.h"
#include "mocks_and_fakes.h"

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
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(nonstd::optional<FoundModuleCollection>(found)));

    ModuleFactory module_factory(scanning, &pool_);
    ASSERT_TRUE(module_factory.create());
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
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(nonstd::optional<FoundModuleCollection>(found)));

    ModuleFactory module_factory(scanning, &pool_);
    ASSERT_TRUE(module_factory.create());
}
