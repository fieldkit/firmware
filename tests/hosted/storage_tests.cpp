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

        log_configure_level(LogLevels::DEBUG);
    }

    void TearDown() override {
        auto erases = 0;
        auto reads = 0;
        auto writes = 0;

        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();

            erases += log.number_of(OperationType::EraseBlock);
            reads += log.number_of(OperationType::Read);
            writes += log.number_of(OperationType::Write);
            // loginfo("bank[%d] %d ops %d reads %d writes %d erase", i, log.size(), reads, writes, erases);
        }

        loginfo("bank[A] %d reads %d writes %d erase", reads, writes, erases);
    }

protected:
    void clear_logs() {
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();
            log.clear();
        }
    }

};

TEST_F(StorageSuite, DisplayStructSizes) {
    loginfo("sizeof(BlockHeader) = %d", sizeof(BlockHeader));
    loginfo("sizeof(BlockTail) = %d", sizeof(BlockTail));
    loginfo("sizeof(RecordHead) = %d", sizeof(RecordHead));
    loginfo("sizeof(RecordTail) = %d", sizeof(RecordTail));
}

TEST_F(StorageSuite, WhenMountingUnformatted) {
    Storage storage{ memory_ };

    ASSERT_FALSE(storage.begin());
    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    uint8_t data[256] = { 0xcc };
    ASSERT_EQ(file_write.write(data, sizeof(data)), (int32_t)sizeof(data));

    ASSERT_TRUE(storage.begin());
}

TEST_F(StorageSuite, WhenMounting) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
}

TEST_F(StorageSuite, AppendingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    uint8_t data[256] = { 0xcc };
    ASSERT_EQ(file_write.write(data, sizeof(data)), (int32_t)sizeof(data));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + sizeof(data) + sizeof(RecordTail);
    ASSERT_EQ(file_write.tail(), expected);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, AppendingRecordsAcrossAPage) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(0);

    auto length = g_.page_size + 128;
    auto data = (uint8_t *)pool_.malloc(length);
    ASSERT_EQ(file.write(data, length), (int32_t)length);

    auto expected = sizeof(BlockHeader) + sizeof(RecordHead) + length + sizeof(RecordTail);
    ASSERT_EQ(file.tail(), expected);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, AppendingMultipleRecords) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    for (auto i = 0; i < 100; ++i) {
        uint8_t data[256] = { (uint8_t)i };
        ASSERT_EQ(file_write.write( data, sizeof(data)), (int32_t)sizeof(data));
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

    auto file0 = storage.file(0);
    auto file1 = storage.file(1);

    uint8_t data0[256] = { (uint8_t)0 };
    ASSERT_EQ(file0.write(data0, sizeof(data0)), (int32_t)sizeof(data0));

    uint8_t data1[256] = { (uint8_t)1 };
    ASSERT_EQ(file1.write(data1, sizeof(data1)), (int32_t)sizeof(data0));

    ASSERT_EQ(file0.record(), (uint32_t)1);
    ASSERT_EQ(file0.size(), (uint32_t)256);

    ASSERT_EQ(file1.record(), (uint32_t)1);
    ASSERT_EQ(file1.size(), (uint32_t)256);
}

TEST_F(StorageSuite, FillingABlock) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(0);

    auto length = 1024;
    auto data = (uint8_t *)pool_.malloc(length);

    for (uint32_t i = 0; i < (g_.block_size / length) + 10; ++i) {
        ASSERT_EQ(file.write(data, length), length);
    }
}

TEST_F(StorageSuite, ReadingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(0);

    uint8_t expected[256] = { 0xcc };
    ASSERT_EQ(file.write(expected, sizeof(expected)), (int32_t)sizeof(expected));

    ASSERT_TRUE(file.seek(0));

    uint8_t actual[256] = { 0x00 };
    ASSERT_EQ(file.read(actual, sizeof(actual)), (int32_t)sizeof(actual));

    ASSERT_EQ(memcmp(expected, actual, sizeof(actual)), 0);
}

TEST_F(StorageSuite, LargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    uint8_t expected[256] = { 0xcc };

    auto bytes_wrote = 0;
    while (bytes_wrote < 10 * 1024 * 1024) {
        ASSERT_EQ(file_write.write(expected, sizeof(expected)), (int32_t)sizeof(expected));

        bytes_wrote += sizeof(expected);
    }

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek(0));

    auto bytes_read = 0;
    while (bytes_read < 10 * 1024 * 1024) {
        uint8_t data[256] = { 0x00 };
        ASSERT_EQ(file_read.read(data, sizeof(data)), (int32_t)sizeof(data));

        ASSERT_EQ(memcmp(expected, data, sizeof(data)), 0);

        bytes_read += sizeof(data);
    }
}

TEST_F(StorageSuite, MultipleLargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);

    uint8_t expected[256] = { 0xcc };

    auto bytes_wrote = 0;
    while (bytes_wrote < 10 * 1024 * 1024) {
        ASSERT_EQ(file0_write.write(expected, sizeof(expected)), (int32_t)sizeof(expected));
        ASSERT_EQ(file1_write.write(expected, sizeof(expected)), (int32_t)sizeof(expected));

        bytes_wrote += sizeof(expected);
    }

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    ASSERT_TRUE(file0_read.seek(0));

    auto bytes_read = 0;
    while (bytes_read < 10 * 1024 * 1024) {
        uint8_t data[256] = { 0x00 };
        ASSERT_EQ(file0_read.read(data, sizeof(data)), (int32_t)sizeof(data));

        ASSERT_EQ(memcmp(expected, data, sizeof(data)), 0);

        bytes_read += sizeof(data);
    }

    auto file1_read = storage.file(1);

    ASSERT_TRUE(file0_read.seek(0));

    bytes_read = 0;
    while (bytes_read < 10 * 1024 * 1024) {
        uint8_t data[256] = { 0x00 };
        ASSERT_EQ(file1_read.read(data, sizeof(data)), (int32_t)sizeof(data));

        ASSERT_EQ(memcmp(expected, data, sizeof(data)), 0);

        bytes_read += sizeof(data);
    }
}
