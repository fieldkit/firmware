#include <fk-data-protocol.h>

#include "tests.h"
#include "patterns.h"
#include "test_modules.h"

#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "clock.h"
#include "protobuf.h"
#include "scanning.h"
#include "modules_bridge.h"
#include "registry.h"
#include "readings.h"

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

        log_configure_level(LogLevels::INFO);
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
    ModuleContext mc{ &gs, module_bus };
    ResolvedModules resolved{ pool };

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(mc, resolved, 0, pool));

    size_t size = 0;
    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record(), &size);

    ASSERT_NE(buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsModuleWithNoReadings) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    ResolvedModules resolved{ pool };

    Readings readings{ get_modmux() };
    resolved.set(0, &fk_test_module_fake_empty);

    ASSERT_TRUE(readings.take_readings(mc, resolved, 0, pool));

    size_t size = 0;
    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record(), &size);

    ASSERT_NE(buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsOneModule) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    ResolvedModules resolved{ pool };
    resolved.set(0, &fk_test_module_fake_1);

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(mc, resolved, 0, pool));

    size_t size = 0;
    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record(), &size);

    ASSERT_NE(buffer, nullptr);
}

TEST_F(ReadingsSuite, TakingReadingsTwoModules) {
    StaticPool<1024> pool{ "readings" };
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ &gs, module_bus };
    ResolvedModules resolved{ pool };
    resolved.set(0, &fk_test_module_fake_1);
    resolved.set(1, &fk_test_module_fake_2);

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(mc, resolved, 0, pool));

    size_t size = 0;
    auto buffer = pool.encode(fk_data_DataRecord_fields, &readings.record(), &size);

    ASSERT_NE(buffer, nullptr);
}

