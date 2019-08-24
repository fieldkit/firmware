#include <fk-data-protocol.h>

#include "storage_suite.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

TEST_F(StorageSuite, DisplayStructSizes) {
    loginfo("sizeof(BlockHeader) = %zd (0x%zx)", sizeof(BlockHeader), sizeof(BlockHeader));
    loginfo("sizeof(BlockTail) = %zd (0x%zx)", sizeof(BlockTail), sizeof(BlockTail));
    loginfo("sizeof(RecordHeader) = %zd (0x%zx)", sizeof(RecordHeader), sizeof(RecordHeader));
    loginfo("sizeof(RecordTail) = %zd (0x%zx)", sizeof(RecordTail), sizeof(RecordTail));
}

TEST_F(StorageSuite, WhenMountingUnformatted) {
    Storage storage{ memory_ };

    ASSERT_FALSE(storage.begin());
    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    uint8_t data[256] = { 0xcc };
    ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());
}

TEST_F(StorageSuite, WhenMounting) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());
}

TEST_F(StorageSuite, ErasingAndStartingOver) {
    uint32_t version1;
    uint32_t version2;

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        write_reading(file_write);
        version1 = storage.version();
    }

    loginfo("version #1 = %" PRIu32, version1);

    // Now start over and write a little data.
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        write_reading(file_write);
        version2 = storage.version();
        FK_ASSERT(version2 != version1);
    }

    loginfo("version #2 = %" PRIu32, version2);

    // Now we open.
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        FK_ASSERT(storage.version() == version2);
    }
}

TEST_F(StorageSuite, AppendingARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    ASSERT_TRUE(file_write.create());

    uint8_t data[256] = { 0xcc };
    ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));

    auto expected = sizeof(BlockHeader) + sizeof(RecordHeader) + sizeof(data) + sizeof(RecordTail);
    ASSERT_EQ(file_write.tail(), expected);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());

    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, WritingFirstFile) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());
    ASSERT_GT(statistics_memory_.statistics().nerases, (uint32_t)0);

    clear_statistics();

    auto file = storage.file(1);
    ASSERT_GT(write_reading(file), (uint32_t)0);
    ASSERT_GT(statistics_memory_.statistics().nerases, (uint32_t)0);
}

TEST_F(StorageSuite, AppendingRecordsAcrossAPage) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(0);
    ASSERT_TRUE(file.create());

    auto length = g_.page_size + 128;
    auto data = (uint8_t *)pool_.malloc(length);
    ASSERT_EQ(file.write(data, length), length);

    auto expected = sizeof(BlockHeader) + sizeof(RecordHeader) + length + sizeof(RecordTail);
    ASSERT_EQ(file.tail(), expected);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.tail(), expected);
}

TEST_F(StorageSuite, AppendingMultipleRecords) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    ASSERT_TRUE(file_write.create());

    for (auto i = 0; i < 100; ++i) {
        uint8_t data[256] = { (uint8_t)i };
        ASSERT_EQ(file_write.write(data, sizeof(data)), sizeof(data));
    }

    ASSERT_EQ(file_write.record(), (uint32_t)100);

    auto actual_tail = file_write.tail();

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.tail(), actual_tail);
    ASSERT_EQ(file_read.record(), (uint32_t)100);
    ASSERT_EQ(file_read.size(), (uint32_t)25600);
}

TEST_F(StorageSuite, AppendingToMultipleFiles) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file0 = storage.file(0);
    ASSERT_TRUE(file0.create());
    auto file1 = storage.file(1);
    ASSERT_TRUE(file1.create());

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
    ASSERT_TRUE(file.create());

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
    ASSERT_TRUE(file.create());

    StaticPattern pattern;

    ASSERT_EQ(file.write(pattern.data, sizeof(pattern.data)), sizeof(pattern.data));

    ASSERT_EQ(file.size(), sizeof(pattern.data));

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    ASSERT_TRUE(file.seek(0));

    ASSERT_EQ(file.size(), sizeof(pattern.data));
    ASSERT_EQ(file.position(), (size_t)0);

    pattern.verify_record(file);
}

// NOTE: Why is this fast?
TEST_F(StorageSuite, SeekingToARecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = 10 * 1024 * 1024;

    ASSERT_TRUE(file_write.create());

    SequentialPattern pattern;
    pattern.write(file_write, size);

    ASSERT_TRUE(memory_->flush());

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

    ASSERT_TRUE(file_write.create());

    SequentialPattern pattern;
    pattern.write(file_write, size);

    ASSERT_TRUE(memory_->flush());

    {
        ASSERT_TRUE(storage.begin());

        auto file_read = storage.file(0);

        ASSERT_TRUE(file_read.seek_end());
        ASSERT_EQ(file_read.position(), size);

        ASSERT_TRUE(file_read.seek(0));
        ASSERT_EQ(file_read.position(), (size_t)0);
    }
}

TEST_F(StorageSuite, ReadingAtEoF) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    ASSERT_TRUE(file_write.create());

    SequentialPattern pattern;
    pattern.write(file_write, 1024);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    uint8_t data[128];
    ASSERT_EQ(file_write.read(data, sizeof(data)), (size_t)0);

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());

    ASSERT_EQ(file_read.read(data, sizeof(data)), (size_t)0);
}

TEST_F(StorageSuite, ReadingBackSingleRecord) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);

    ASSERT_TRUE(file_write.create());

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
    record.log.uptime = 9359854936;
    record.log.time = 0;
    record.log.level = (uint32_t)LogLevels::INFO;
    record.log.facility.arg = (void *)"facility";
    record.log.facility.funcs.encode = pb_encode_string;
    record.log.message.arg = (void *)"message";
    record.log.message.funcs.encode = pb_encode_string;

    auto file_write = storage.file(0);
    ASSERT_TRUE(file_write.create());

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
    ASSERT_TRUE(file_write.create());

    for (auto i = 0; i < 10; ++i) {
        pattern.write(file_write, 256);
    }

    size_t size = 256 * 10;
    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);
}

TEST_F(StorageSuite, WritingOncePerOpenHasCorrectRecordNumbers) {
    Storage storage{ memory_ };
    StaticPattern pattern;

    {
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        pattern.write(file_write, 256);
        ASSERT_EQ(file_write.record(), (uint32_t)1);
    }

    ASSERT_TRUE(memory_->flush());

    for (auto i = 0; i < 9; ++i) {
        ASSERT_TRUE(storage.begin());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.seek_end());
        pattern.write(file_write, 256);
        ASSERT_EQ(file_write.record(), (uint32_t)2 + i);
        ASSERT_TRUE(memory_->flush());
    }
}

TEST_F(StorageSuite, ClearingAfterWritingToFiles) {
    Storage storage{ memory_ };
    StaticPattern pattern;

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    size_t size = 256 * 10;
    ASSERT_TRUE(file_write.create());
    pattern.write(file_write, size);
    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(memory_->flush());

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

    ASSERT_TRUE(file_write.create());

    for (auto i = 0; i < 1000; ++i) {
        auto wrote = write_reading(file_write);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.position(), size);

    ASSERT_TRUE(file_read.seek(0));
    ASSERT_EQ(file_read.position(), (size_t)0);
}

TEST_F(StorageSuite, WritingThroughABlockClosingAndWritingAnother) {
    Storage storage{ memory_ };

    ASSERT_TRUE(storage.clear());

    auto file_write1 = storage.file(0);
    auto size = file_write1.size();

    ASSERT_TRUE(file_write1.create());

    for (auto i = 0; i < 600; ++i) {
        auto wrote = write_reading(file_write1);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_write2 = storage.file(0);
    ASSERT_TRUE(file_write2.seek_end());

    for (auto i = 0; i < 1500; ++i) {
        auto wrote = write_reading(file_write2);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.position(), size);

    ASSERT_TRUE(file_read.seek(0));
    ASSERT_EQ(file_read.position(), (size_t)0);
}

TEST_F(StorageSuite, SeekingWithinOneBlock) {
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.create());
        auto wrote = write_reading(file0);
        ASSERT_GT(wrote, (uint32_t)0);
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.seek(0));
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.seek_end());
    }
}

TEST_F(StorageSuite, SeekingBeginningUnwrittenSecondFile) {
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.create());
        auto wrote = write_reading(file0);
        ASSERT_GT(wrote, (uint32_t)0);
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.seek(0));
        auto file1 = storage.file(1);
        ASSERT_FALSE(file1.seek(0));
    }
}

TEST_F(StorageSuite, SeekingBeginningUnwrittenSecondFileWhenWritingSecond) {
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file1 = storage.file(1);
        ASSERT_TRUE(file1.create());
        auto wrote = write_reading(file1);
        ASSERT_GT(wrote, (uint32_t)0);
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file0 = storage.file(0);
        ASSERT_FALSE(file0.seek(0));
        auto file1 = storage.file(1);
        ASSERT_TRUE(file1.seek(0));
    }
}

TEST_F(StorageSuite, SeekingEndUnwrittenSecondFile) {
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.create());
        auto wrote = write_reading(file0);
        ASSERT_GT(wrote, (uint32_t)0);
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file0 = storage.file(0);
        ASSERT_TRUE(file0.seek_end());
        auto file1 = storage.file(1);
        ASSERT_FALSE(file1.seek_end());
    }
}

TEST_F(StorageSuite, SeekingSmallSecondFile) {
    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.clear());
        auto file1 = storage.file(1);
        ASSERT_TRUE(file1.create());
        auto wrote = write_reading(file1);
        ASSERT_GT(wrote, (uint32_t)0);
    }

    {
        Storage storage{ memory_ };
        ASSERT_TRUE(storage.begin());
        auto file1 = storage.file(1);
        ASSERT_TRUE(file1.seek(0));
    }
}

TEST_F(StorageSuite, SeekingToEndOfFileBeforeWriting) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());
    ASSERT_GT(statistics_memory_.statistics().nerases, (uint32_t)0);

    clear_statistics();

    auto file = storage.file(0);
    ASSERT_FALSE(file.seek_end());
    ASSERT_TRUE(file.create());
    ASSERT_GT(write_reading(file), (uint32_t)0);

    ASSERT_GT(statistics_memory_.statistics().nerases, (uint32_t)0);
}

TEST_F(StorageSuite, SeekingSmallSecondFileBeforeWriting) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());

    auto file1n1 = storage.file(1);
    ASSERT_FALSE(file1n1.seek(0));
    ASSERT_FALSE(file1n1.seek_end());

    {
        auto file1n2 = storage.file(1);
        ASSERT_FALSE(file1n2.seek(0));
        ASSERT_FALSE(file1n2.seek_end());
        ASSERT_TRUE(file1n2.create());
        ASSERT_GT(write_reading(file1n2), (uint32_t)0);
    }
    {
        auto file1n2 = storage.file(1);
        ASSERT_TRUE(file1n2.seek(0));
        ASSERT_TRUE(file1n2.seek_end());
        ASSERT_GT(write_reading(file1n2), (uint32_t)0);
    }
}

TEST_F(StorageSuite, SeekingSetsPositionCorrectly) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());

    auto file = storage.file(1);
    ASSERT_FALSE(file.seek(0));
    ASSERT_FALSE(file.seek_end());
    ASSERT_GT(write_reading(file), (uint32_t)0);

    auto position1 = file.position();

    ASSERT_TRUE(file.seek(0));
    ASSERT_TRUE(file.seek_end());

    auto position2 = file.position();
    ASSERT_EQ(position2, position1);
}

TEST_F(StorageSuite, SeekingAndReadingAndSeekingSetsPositionCorrectly) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());

    auto file = storage.file(1);
    ASSERT_FALSE(file.seek(0));
    ASSERT_FALSE(file.seek_end());
    ASSERT_TRUE(file.create());
    ASSERT_GT(write_reading(file), (uint32_t)0);

    auto position1 = file.position();

    // This is the start of the current record, which is still record 0.
    auto found = file.reference();
    uint8_t buffer[256];
    ASSERT_TRUE(file.seek(0));
    ASSERT_GT(file.read(buffer, sizeof(buffer)), (uint32_t)0);
    file.seek(found);
    ASSERT_GT(file.read(buffer, sizeof(buffer)), (uint32_t)0);

    auto position2 = file.position();
    ASSERT_EQ(position2, position1);
}

TEST_F(StorageSuite, SeekingToEndOfFileWithNoRecordsInLastBlock) {
    // NOTE: This will mean that our record_address in the SeekValue is wrong.
    // This should technically be pretty hard to cause because when we are
    // writing a record we would immediately start a new block and write the new
    // record to the new block. I think the only way for this to happen would be
    // some kind of failure when writing. Otherwise, all allocated blocks should
    // get a first record.
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());
}

TEST_F(StorageSuite, SeekingToBeginningWhenBeginningSecondFileAFewBlocksAfterFirstFile) {
    Storage storage{ memory_ };
    ASSERT_TRUE(storage.clear());

    auto file_write0 = storage.file(0);
    ASSERT_TRUE(file_write0.create());
    auto size = (uint32_t)0;
    while (size < g_.block_size * 2) {
        auto wrote = write_reading(file_write0);
        size += wrote;
    }
    ASSERT_EQ(file_write0.position() / g_.block_size, (uint32_t)2);

    auto file_write1 = storage.file(1);
    ASSERT_TRUE(file_write1.create());
    auto wrote1 = write_reading(file_write1);

    auto file_read0 = storage.file(0);
    ASSERT_TRUE(file_read0.seek_end());
    ASSERT_EQ(file_read0.size(), size);

    auto file_read1 = storage.file(1);
    ASSERT_TRUE(file_read1.seek_end());
    ASSERT_EQ(file_read1.record(), (uint32_t)1);
    ASSERT_EQ(file_read1.size(), wrote1);
}
