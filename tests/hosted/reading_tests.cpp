#include <fk-data-protocol.h>

#include "tests.h"
#include "patterns.h"
#include "test_modules.h"
#include "pool.h"
#include "protobuf.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "clock.h"
#include "readings.h"
#include "modules/scanning.h"
#include "modules/registry.h"
#include "modules/bridge/modules_bridge.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class ReadingsSuite : public ::testing::Test {
protected:
    StandardPool pool_{ "storage" };
    LinuxDataMemory *banks_[MemoryFactory::NumberOfDataMemoryBanks] = { nullptr };
    DataMemory *data_memory_{ nullptr };
    StatisticsMemory statistics_memory_{ data_memory_ };
    DataMemory *memory_{ &statistics_memory_ };
    FlashGeometry g_;

protected:
    void SetUp() override {
        auto opaque = MemoryFactory::get_data_memory_banks();
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            banks_[i] = reinterpret_cast<LinuxDataMemory*>(opaque[i]);
        }

        data_memory_ = MemoryFactory::get_data_memory();
        statistics_memory_ = StatisticsMemory{ data_memory_ };

        g_ = memory_->geometry();
        pool_.clear();

        memory_->begin();
    }

    void TearDown() override {
    }

protected:
    void enable_debug() {
        log_configure_level(LogLevels::DEBUG);
    }

    void enable_trace() {
        log_configure_level(LogLevels::TRACE);
    }

};

TEST_F(ReadingsSuite, TakingReadingsNoModules) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);

    Readings readings{ get_modmux(), pool_ };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsModuleWithNoReadings) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(0) },
        .meta = &fk_test_module_fake_empty,
        .module_instance = fk_test_module_fake_empty.ctor(pool),
    });

    Readings readings{ get_modmux(), pool_ };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsOneModule) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(0) },
        .meta = &fk_test_module_fake_1,
        .module_instance = fk_test_module_fake_1.ctor(pool),
    });

    Readings readings{ get_modmux(), pool_ };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsTwoModules) {
    StaticPool<2048> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(0) },
        .meta = &fk_test_module_fake_1,
        .module_instance = fk_test_module_fake_1.ctor(pool),
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(1) },
        .meta = &fk_test_module_fake_2,
        .module_instance = fk_test_module_fake_2.ctor(pool),
    });

    Readings readings{ get_modmux(), pool_ };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsFourModulesEverythingWorks) {
    StandardPool pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(0) },
        .meta = &fk_test_module_fake_1,
        .module_instance = fk_test_module_fake_1.ctor(pool),
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(1) },
        .meta = &fk_test_module_fake_1,
        .module_instance = fk_test_module_fake_1.ctor(pool),
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(2) },
        .meta = &fk_test_module_fake_2,
        .module_instance = fk_test_module_fake_2.ctor(pool),
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(3) },
        .meta = &fk_test_module_fake_2,
        .module_instance = fk_test_module_fake_2.ctor(pool),
    });

    Readings readings{ get_modmux(), pool_ };
    auto taken = readings.take_readings(ctx, resolved, pool);
    ASSERT_TRUE(taken);

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());
    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);

    ASSERT_EQ(taken->size(), 4u);
}

TEST_F(ReadingsSuite, TakingReadingsFourModulesAndOneFailsToGetReadings) {
    StandardPool pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };
    ConstructedModulesCollection resolved(pool);
    FakeModule *modules[] = {
        reinterpret_cast<FakeModule*>(fk_test_module_fake_1.ctor(pool)),
        reinterpret_cast<FakeModule*>(fk_test_module_fake_empty.ctor(pool)),
        reinterpret_cast<FakeModule*>(fk_test_module_fake_2.ctor(pool)),
        reinterpret_cast<FakeModule*>(fk_test_module_fake_2.ctor(pool)),
    };
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(0) },
        .meta = &fk_test_module_fake_1,
        .module_instance = modules[0],
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(1) },
        .meta = &fk_test_module_fake_empty,
        .module_instance = modules[1],
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(2) },
        .meta = &fk_test_module_fake_2,
        .module_instance = modules[2],
    });
    resolved.emplace(ConstructedModule{
        .found = { .position = ModulePosition::from(3) },
        .meta = &fk_test_module_fake_2,
        .module_instance = modules[3],
    });

    Readings readings{ get_modmux(), pool_ };
    auto taken = readings.take_readings(ctx, resolved, pool);
    ASSERT_TRUE(taken);

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());
    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);

    ASSERT_EQ(taken->size(), 4u);
}
