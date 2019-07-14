#include <fk-data-protocol.h>

#include "tests.h"
#include "pool.h"
#include "hal/linux/linux.h"
#include "storage.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

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

TEST_F(StorageSuite, DisplayStructSizes) {
    loginfo("sizeof(BlockHeader) = %d", sizeof(BlockHeader));
    loginfo("sizeof(BlockTail) = %d", sizeof(BlockTail));
    loginfo("sizeof(RecordHead) = %d", sizeof(RecordHead));
    loginfo("sizeof(RecordTail) = %d", sizeof(RecordTail));
}

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

    auto file_write = storage.file(0);

    uint8_t data[256] = { 0xcc };
    ASSERT_TRUE(file_write.write(data, sizeof(data)));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + sizeof(data) + sizeof(RecordTail);
    ASSERT_EQ(file_write.tail(), expected);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, AppendingRecordsAcrossAPage) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto file = storage.file(0);

    auto length = g_.page_size + 128;
    auto data = (uint8_t *)pool_.malloc(length);
    ASSERT_TRUE(file.write(data, length));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + length + sizeof(RecordTail);
    ASSERT_EQ(file.tail(), expected);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, AppendingMultipleRecords) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto file_write = storage.file(0);

    for (auto i = 0; i < 100; ++i) {
        uint8_t data[256] = { (uint8_t)i };
        ASSERT_TRUE(file_write.write( data, sizeof(data)));
    }

    ASSERT_EQ(file_write.record(), (uint32_t)100);

    auto actual_tail = file_write.tail();

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_EQ(file_read.tail(), actual_tail);
    ASSERT_EQ(file_read.record(), (uint32_t)100);
    ASSERT_EQ(file_read.size(), (uint32_t)25600);
}

TEST_F(StorageSuite, AppendingToMultipleFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto file0 = storage.file(0);
    auto file1 = storage.file(1);

    uint8_t data0[256] = { (uint8_t)0 };
    ASSERT_TRUE(file0.write(data0, sizeof(data0)));

    uint8_t data1[256] = { (uint8_t)1 };
    ASSERT_TRUE(file1.write(data1, sizeof(data1)));

    ASSERT_EQ(file0.record(), (uint32_t)1);
    ASSERT_EQ(file0.size(), (uint32_t)256);

    ASSERT_EQ(file1.record(), (uint32_t)1);
    ASSERT_EQ(file1.size(), (uint32_t)256);
}

TEST_F(StorageSuite, FillingABlock) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto file = storage.file(0);

    auto length = 1024;
    auto data = (uint8_t *)pool_.malloc(length);

    for (uint32_t i = 0; i < (g_.block_size / length) + 10; ++i) {
        ASSERT_TRUE(file.write(data, length));
    }
}

TEST_F(StorageSuite, ReadingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(storage.begin());

    auto file = storage.file(0);

    uint8_t expected[256] = { 0xcc };

    ASSERT_TRUE(file.write(expected, sizeof(expected)));

    ASSERT_TRUE(file.seek(0));

    uint8_t actual[256] = { 0x00 };
    ASSERT_TRUE(file.read(actual, sizeof(actual)));

    ASSERT_EQ(memcmp(expected, actual, sizeof(actual)), 0);
}
