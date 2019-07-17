#include <fk-data-protocol.h>

#include "tests.h"
#include "pool.h"
#include "hal/linux/linux.h"
#include "storage.h"
#include "patterns.h"
#include "protobuf.h"

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

        log_configure_level(LogLevels::INFO);
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
        }

        loginfo("bank[A] %d reads %d writes %d erase", reads, writes, erases);
    }

protected:
    void verbose() {
        log_configure_level(LogLevels::TRACE);
    }

    void clear_logs() {
        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();
            log.clear();
        }
    }

};

TEST_F(StorageSuite, DisplayStructSizes) {
    loginfo("sizeof(BlockHeader) = %d (0x%x)", sizeof(BlockHeader), sizeof(BlockHeader));
    loginfo("sizeof(BlockTail) = %d (0x%x)", sizeof(BlockTail), sizeof(BlockTail));
    loginfo("sizeof(RecordHeader) = %d (0x%x)", sizeof(RecordHeader), sizeof(RecordHeader));
    loginfo("sizeof(RecordTail) = %d (0x%x)", sizeof(RecordTail), sizeof(RecordTail));
}

TEST_F(StorageSuite, WhenMountingUnformatted) {
    Storage storage{ memory_ };

    ASSERT_FALSE(storage.begin());
    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    uint8_t data[256] = { 0xcc };
    ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));

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
    ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHeader) + sizeof(data) + sizeof(RecordTail);
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
    ASSERT_EQ(file.write(data, length), length);

    auto expected = sizeof(BlockHeader) + sizeof(RecordHeader) + length + sizeof(RecordTail);
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
        ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));
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

    uint8_t data0[256];
    memset(data0, 0x00, sizeof(data0));
    ASSERT_EQ(file0.write(data0, sizeof(data0)), sizeof(data0));

    uint8_t data1[256];
    memset(data1, 0x01, sizeof(data1));
    ASSERT_EQ(file1.write(data1, sizeof(data1)), sizeof(data0));

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
    memset(data, 0xcd, length);

    for (uint32_t i = 0; i < (g_.block_size / length) + 10; ++i) {
        ASSERT_EQ(file.write(data, length), (size_t)length);
    }
}

TEST_F(StorageSuite, ReadingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(0);

    StaticPattern pattern;

    ASSERT_EQ(file.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));

    ASSERT_TRUE(storage.begin());

    ASSERT_TRUE(file.seek(0));

    pattern.verify_record(file);
}

TEST_F(StorageSuite, LargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = 10 * 1024 * 1024;

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    pattern.verify(file_read, size);
}

TEST_F(StorageSuite, MultipleLargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);
    auto size = 10 * 1024 * 1024;

    StaticPattern pattern;

    auto bytes_wrote = 0;
    while (bytes_wrote < size) {
        ASSERT_EQ(file0_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));
        ASSERT_EQ(file1_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));

        bytes_wrote += sizeof(pattern.data);
    }

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    pattern.verify(file0_read, size);

    auto file1_read = storage.file(1);

    pattern.verify(file1_read, size);
}

TEST_F(StorageSuite, MultipleLargeFilesOneMuchSmaller) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);
    auto size = 10 * 1024 * 1024;

    StaticPattern pattern;

    auto bytes_wrote = 0;
    while (bytes_wrote < size) {
        ASSERT_EQ(file0_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));

        if (bytes_wrote % 4096 == 0) {
            ASSERT_EQ(file1_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));
        }

        bytes_wrote += sizeof(pattern.data);
    }

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    pattern.verify(file0_read, size);

    auto file1_read = storage.file(1);

    pattern.verify(file1_read, size / 4096);
}

TEST_F(StorageSuite, SeekingToARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = 10 * 1024 * 1024;

    SequentialPattern pattern;
    pattern.write(file_write, size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek(100));
    pattern.verify_record(file_read, 100);

    ASSERT_TRUE(file_read.seek(size / 256 / 2));
    pattern.verify_record(file_read, (size / 256 / 2) & 0xff);
}

TEST_F(StorageSuite, ReadingAtEoF) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    SequentialPattern pattern;
    pattern.write(file_write, 1024);

    ASSERT_TRUE(storage.begin());

    uint8_t data[128];
    ASSERT_EQ(file_write.read(data, sizeof(data)), (size_t)0);

    auto file_read = storage.file(0);

    loginfo("seek");

    ASSERT_TRUE(file_read.seek(LastRecord));

    ASSERT_EQ(file_read.read(data, sizeof(data)), (size_t)0);
}

TEST_F(StorageSuite, ReadingBackSingleRecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    SequentialPattern pattern;
    pattern.write(file_write, 256);

    auto file_read = storage.file(0);

    file_read.seek(0);

    pattern.verify_record(file_read, 0x00);
}

TEST_F(StorageSuite, WritingProtobuf) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.log.uptime = fk_uptime();
    record.log.time = 0;
    record.log.level = (uint32_t)LogLevels::INFO;
    record.log.facility.arg = (void *)"facility";
    record.log.facility.funcs.encode = pb_encode_string;
    record.log.message.arg = (void *)"message";
    record.log.message.funcs.encode = pb_encode_string;

    auto file_write = storage.file(0);

    ASSERT_EQ(file_write.write(&record), (size_t)28);

    auto file_read = storage.file(0);

    file_read.seek(0);

    record = fk_data_DataRecord_init_default;
    record.log.facility.arg = (void *)&pool_;
    record.log.facility.funcs.decode = pb_decode_string;
    record.log.message.arg = (void *)&pool_;
    record.log.message.funcs.decode = pb_decode_string;

    ASSERT_EQ(file_read.read(&record), (size_t)28);
}
