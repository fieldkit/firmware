#include "tests.h"

#include "module_factory.h"
#include "mocks_and_fakes.h"
#include "readings_taker.h"
#include "state.h"

#include "storage_suite.h"
#include "test_modules.h"

using testing::Return;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::_;
using namespace fk;

class ReadingsTakingSuite : public StorageSuite {
protected:
    void SetUp() override {
        StorageSuite::SetUp();
        fk_modules_builtin_register(&fk_test_module_fake_empty);
        fk_modules_builtin_register(&fk_test_module_fake_1);
        fk_modules_builtin_register(&fk_test_module_fake_2);
    }

    void TearDown() override {
        fk_modules_builtin_clear();
        StorageSuite::TearDown();
    }
};

TEST_F(ReadingsTakingSuite, WithNoModules) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    Storage storage{ memory_ };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(nonstd::optional<FoundModuleCollection>(found)));

    ReadingsTaker readings_taker{ scanning, storage, get_modmux() };
    ASSERT_TRUE(readings_taker.take(mc, pool_));
}

TEST_F(ReadingsTakingSuite, BasicSingleModule) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    Storage storage{ memory_ };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);
    found.emplace_back(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(nonstd::optional<FoundModuleCollection>(found)));

    ReadingsTaker readings_taker{ scanning, storage, get_modmux() };
    ASSERT_TRUE(readings_taker.take(mc, pool_));
}

TEST_F(ReadingsTakingSuite, BasicTwoModules) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    Storage storage{ memory_ };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);
    found.emplace_back(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });
    found.emplace_back(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x03,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(nonstd::optional<FoundModuleCollection>(found)));

    ReadingsTaker readings_taker{ scanning, storage, get_modmux() };
    ASSERT_TRUE(readings_taker.take(mc, pool_));
}
