#include <fk-data-protocol.h>

#include "tests.h"
#include "pool.h"
#include "hal/linux/linux.h"
#include "storage.h"

using namespace fk;

class StorageSuite : public ::testing::Test {
protected:
    MallocPool pool_{ "storage", 1024 * 100 };
    LinuxDataMemory *banks_[MemoryFactory::NumberOfDataMemoryBanks] = { nullptr };
    DataMemory *memory_{ nullptr };
    flash_geometry_t g_;

protected:
    void SetUp() override {
        auto opaque = MemoryFactory::get_data_memory_banks();
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            banks_[i] = reinterpret_cast<LinuxDataMemory*>(opaque[i]);
        }

        memory_ = MemoryFactory::get_data_memory();
        g_ = memory_->geometry();
        pool_.clear();

        memory_->begin();

        log_configure_level(LogLevels::TRACE);
    }

};

TEST_F(StorageSuite, DISABLED_WhenMountingUnformatted) {
    Storage storage{ memory_ };

    ASSERT_FALSE(storage.begin());
    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());
}

TEST_F(StorageSuite, WhenMounting) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());
}

TEST_F(StorageSuite, AppendingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    uint8_t data[256] = { 0xcc };

    ASSERT_TRUE(storage.append(0, data, sizeof(data)));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + sizeof(data) + sizeof(RecordTail);
    ASSERT_EQ(storage.tail(0), expected);

    ASSERT_TRUE(storage.begin());

    ASSERT_EQ(storage.tail(0), expected);
}

TEST_F(StorageSuite, AppendingRecordsAcrossAPage) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto length = g_.page_size + 128;
    auto data = (uint8_t *)pool_.malloc(length);
    ASSERT_TRUE(storage.append(0, data, length));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + length + sizeof(RecordTail);
    ASSERT_EQ(storage.tail(0), expected);

    ASSERT_TRUE(storage.begin());

    ASSERT_EQ(storage.tail(0), expected);
}

TEST_F(StorageSuite, AppendingMultipleRecords) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    for (auto i = 0; i < 100; ++i) {
        uint8_t data[256] = { (uint8_t)i };
        ASSERT_TRUE(storage.append(0, data, sizeof(data)));
    }

    ASSERT_EQ(storage.sequence(0), (uint32_t)100);
    ASSERT_EQ(storage.sequence(1), (uint32_t)0);

    auto actual_tail = storage.tail(0);

    ASSERT_TRUE(storage.begin());

    ASSERT_EQ(storage.tail(0), actual_tail);

    ASSERT_EQ(storage.sequence(0), (uint32_t)100);
    ASSERT_EQ(storage.size(0), (uint32_t)25600);

    ASSERT_EQ(storage.sequence(1), (uint32_t)0);
    ASSERT_EQ(storage.size(1), (uint32_t)0);
}

TEST_F(StorageSuite, AppendingToMultipleFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    uint8_t data0[256] = { (uint8_t)0 };
    ASSERT_TRUE(storage.append(0, data0, sizeof(data0)));

    uint8_t data1[256] = { (uint8_t)1 };
    ASSERT_TRUE(storage.append(1, data1, sizeof(data1)));

    ASSERT_EQ(storage.sequence(0), (uint32_t)1);
    ASSERT_EQ(storage.size(0), (uint32_t)256);

    ASSERT_EQ(storage.sequence(1), (uint32_t)1);
    ASSERT_EQ(storage.size(1), (uint32_t)256);
}

TEST_F(StorageSuite, FillingABlock) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto length = 1024;
    auto data = (uint8_t *)pool_.malloc(length);

    for (uint32_t i = 0; i < (g_.block_size / length) + 10; ++i) {
        ASSERT_TRUE(storage.append(0, data, length));
    }
}
