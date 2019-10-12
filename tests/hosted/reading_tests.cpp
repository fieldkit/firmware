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
    MallocPool pool_{ "storage", 1024 * 100 };
    LinuxDataMemory *banks_[MemoryFactory::NumberOfDataMemoryBanks] = { nullptr };
    DataMemory *data_memory_{ nullptr };
    StatisticsMemory statistics_memory_{ data_memory_ };
    DataMemory *memory_{ &statistics_memory_ };
    flash_geometry_t g_;

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
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ConstructedModulesCollection resolved(pool);

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, 0, 0, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsModuleWithNoReadings) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace_back(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_empty,
        .module = fk_test_module_fake_empty.ctor(pool),
    });

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, 0, 0, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsOneModule) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace_back(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_1,
        .module = fk_test_module_fake_1.ctor(pool),
    });

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, 0, 0, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsTwoModules) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus };
    ConstructedModulesCollection resolved(pool);
    resolved.emplace_back(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_1,
        .module = fk_test_module_fake_1.ctor(pool),
    });
    resolved.emplace_back(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_2,
        .module = fk_test_module_fake_2.ctor(pool),
    });

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, 0, 0, pool));

    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record());

    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(buffer->buffer, nullptr);
}

