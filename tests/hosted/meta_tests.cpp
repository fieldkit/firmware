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
#include "meta.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class MetaLogSuite : public ::testing::Test {
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

TEST_F(MetaLogSuite, OpeningEmptyFile) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

    size_t size = 0;
    auto buffer = pool.encode(fk_data_DataRecord_fields, &meta_log.record(), &size);

    ASSERT_NE(buffer, nullptr);
}

