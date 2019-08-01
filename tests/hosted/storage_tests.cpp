#include <fk-data-protocol.h>

#include "tests.h"
#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "patterns.h"
#include "protobuf.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

static size_t write_reading(File &file);

class StorageSuite : public ::testing::Test {
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
        auto erases = 0;
        auto reads = 0;
        auto writes = 0;

        for (size_t i = 0; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
            auto &log = banks_[i]->log();

            erases += log.number_of(OperationType::EraseBlock);
            reads += log.number_of(OperationType::Read);
            writes += log.number_of(OperationType::Write);
        }

        if (false) {
            loginfo("bank[A] %d reads %d writes %d erase", reads, writes, erases);
        }

        statistics_memory_.log_statistics();
    }

protected:
    void enable_debug() {
        log_configure_level(LogLevels::DEBUG);
    }

    void enable_trace() {
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

    ASSERT_EQ(file.size(), sizeof(pattern.data));

    ASSERT_TRUE(storage.begin());

    ASSERT_TRUE(file.seek(0));

    ASSERT_EQ(file.size(), sizeof(pattern.data));
    ASSERT_EQ(file.position(), (size_t)0);

    pattern.verify_record(file);
}

TEST_F(StorageSuite, LargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    size_t size = 10 * 1024 * 1024;

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), (size_t)0);

    pattern.verify(file_read, size);

    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), size);
}

TEST_F(StorageSuite, MultipleLargeFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);
    auto size = 10 * 1024 * 1024;

    ASSERT_EQ(file0_write.position(), (size_t)0);
    ASSERT_EQ(file0_write.size(), (size_t)0);
    ASSERT_EQ(file1_write.position(), (size_t)0);
    ASSERT_EQ(file1_write.size(), (size_t)0);

    StaticPattern pattern;

    auto counter = 0;
    auto bytes_wrote = 0;
    while (bytes_wrote < size) {
        ASSERT_EQ(file0_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));
        ASSERT_EQ(file1_write.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));

        bytes_wrote += sizeof(pattern.data);

        counter++;

        ASSERT_EQ(file0_write.position(), sizeof(pattern.data) * counter);
        ASSERT_EQ(file1_write.position(), sizeof(pattern.data) * counter);
    }

    ASSERT_EQ(file0_write.position(), (size_t)size);
    ASSERT_EQ(file0_write.size(), (size_t)size);
    ASSERT_EQ(file1_write.position(), (size_t)size);
    ASSERT_EQ(file1_write.size(), (size_t)size);

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    ASSERT_EQ(file0_read.position(), (size_t)0);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    pattern.verify(file0_read, size);

    ASSERT_EQ(file0_read.position(), (size_t)size);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    auto file1_read = storage.file(1);

    ASSERT_EQ(file1_read.position(), (size_t)0);
    ASSERT_EQ(file1_read.size(), (size_t)size);

    pattern.verify(file1_read, size);

    ASSERT_EQ(file1_read.position(), (size_t)size);
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

    auto smaller_size = (size_t)(size / 4096) * sizeof(pattern.data);

    ASSERT_EQ(file0_write.position(), (size_t)size);
    ASSERT_EQ(file0_write.size(), (size_t)size);
    ASSERT_EQ(file1_write.position(), smaller_size);
    ASSERT_EQ(file1_write.size(), smaller_size);

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    ASSERT_EQ(file0_read.position(), (size_t)0);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    pattern.verify(file0_read, size);

    auto file1_read = storage.file(1);

    ASSERT_EQ(file1_read.position(), (size_t)0);
    ASSERT_EQ(file1_read.size(), smaller_size);

    pattern.verify(file1_read, smaller_size);

    ASSERT_EQ(file1_read.position(), smaller_size);
}

TEST_F(StorageSuite, SeekingToARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = 10 * 1024 * 1024;

    SequentialPattern pattern;
    pattern.write(file_write, size);

    {
        ASSERT_TRUE(storage.begin());

        auto file_read = storage.file(0);

        ASSERT_TRUE(file_read.seek(size / 256 / 2));
        ASSERT_EQ(file_read.position(), (size_t)((size / 256) / 2) * 256);
        pattern.verify_record(file_read, (size / 256 / 2) & 0xff);

        ASSERT_TRUE(file_read.seek(100));
        ASSERT_EQ(file_read.position(), (size_t)(100 * 256));
        pattern.verify_record(file_read, 100);
    }

    {
        ASSERT_TRUE(storage.begin());

        auto file_read = storage.file(0);

        ASSERT_TRUE(file_read.seek(100));
        ASSERT_EQ(file_read.position(), (size_t)(100 * 256));
        pattern.verify_record(file_read, 100);

        ASSERT_TRUE(file_read.seek(size / 256 / 2));
        ASSERT_EQ(file_read.position(), (size_t)((size / 256) / 2) * 256);
        pattern.verify_record(file_read, (size / 256 / 2) & 0xff);
    }
}

TEST_F(StorageSuite, SeekingToARecordWithSmallerFile) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = (size_t)256 * 1000;

    SequentialPattern pattern;
    pattern.write(file_write, size);

    {
        ASSERT_TRUE(storage.begin());

        auto file_read = storage.file(0);

        ASSERT_TRUE(file_read.seek(LastRecord));
        ASSERT_EQ(file_read.position(), size);

        ASSERT_TRUE(file_read.seek(0));
        ASSERT_EQ(file_read.position(), (size_t)0);
    }
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

    ASSERT_EQ(file_write.write(&record, fk_data_DataRecord_fields), (size_t)29);

    auto file_read = storage.file(0);

    file_read.seek(0);

    record = fk_data_DataRecord_init_default;
    record.log.facility.arg = (void *)&pool_;
    record.log.facility.funcs.decode = pb_decode_string;
    record.log.message.arg = (void *)&pool_;
    record.log.message.funcs.decode = pb_decode_string;

    ASSERT_EQ(file_read.read(&record, fk_data_DataRecord_fields), (size_t)29);
}

TEST_F(StorageSuite, WritingSequentiallyHasCorrectRecordNumbers) {
    Storage storage{ memory_ };
    StaticPattern pattern;

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    for (auto i = 0; i < 10; ++i) {
        pattern.write(file_write, 256);
    }

    size_t size = 256 * 10;
    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
}

TEST_F(StorageSuite, WritingOncePerOpenHasCorrectRecordNumbers) {
    Storage storage{ memory_ };
    StaticPattern pattern;

    {
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        pattern.write(file_write, 256);
        ASSERT_EQ(file_write.record(), (uint32_t)1);
    }

    for (auto i = 0; i < 9; ++i) {
        ASSERT_TRUE(storage.begin());
        auto file_write = storage.file(0);
        pattern.write(file_write, 256);
        ASSERT_EQ(file_write.record(), (uint32_t)2 + i);
    }
}

TEST_F(StorageSuite, ClearingAfterWritingToFiles) {
    Storage storage{ memory_ };
    StaticPattern pattern;

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    size_t size = 256 * 10;
    pattern.write(file_write, size);
    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.clear());
    file_write = storage.file(0);
    pattern.write(file_write, size);
    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);
}

TEST_F(StorageSuite, SeekingToAReading) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = file_write.size();

    for (auto i = 0; i < 1000; ++i) {
        auto wrote = write_reading(file_write);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek(LastRecord));
    ASSERT_EQ(file_read.position(), size);

    ASSERT_TRUE(file_read.seek(0));
    ASSERT_EQ(file_read.position(), (size_t)0);
}

TEST_F(StorageSuite, WritingThroughABlockClosingAndWritingAnother) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write1 = storage.file(0);
    auto size = file_write1.size();

    for (auto i = 0; i < 600; ++i) {
        auto wrote = write_reading(file_write1);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    ASSERT_TRUE(storage.begin());

    auto file_write2 = storage.file(0);

    for (auto i = 0; i < 1500; ++i) {
        auto wrote = write_reading(file_write2);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek(LastRecord));
    ASSERT_EQ(file_read.position(), size);

    ASSERT_TRUE(file_read.seek(0));
    ASSERT_EQ(file_read.position(), (size_t)0);
}

static size_t write_reading(File &file) {
    fk_data_SensorAndValue readings[] = {
        { 0, (float)random() },
        { 1, (float)random() },
        { 2, (float)random() },
        { 3, (float)random() },
        { 4, (float)random() },
        { 5, (float)random() },
        { 6, (float)random() },
        { 7, (float)random() },
        { 8, (float)random() },
        { 9, (float)random() },
    };

    pb_array_t readings_array = {
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_SensorGroup groups[] = {
        {
            .module = 0,
            .readings = {
                .funcs = {
                    .encode = pb_encode_array,
                },
                .arg = &readings_array,
            },
        }
    };

    pb_array_t sensor_groups_array = {
        .length = (size_t)1,
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = &groups,
        .fields = fk_data_SensorGroup_fields,
    };

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.time = fk_uptime();
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.location.fix = 0;
    record.readings.location.time = fk_uptime();
    record.readings.location.longitude = -118.2709223;
    record.readings.location.latitude = 34.0318047;
    record.readings.location.altitude = 100.0f;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.readings.sensorGroups.arg = &sensor_groups_array;

    auto wrote = file.write(&record, fk_data_DataRecord_fields);
    if (wrote == 0) {
        logerror("error saving readings");
        return 0;
    }

    return wrote;
}
