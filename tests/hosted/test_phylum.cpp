#include "tests.h"
#include "worker.h"
#include "collections.h"
#include "storage/phylum.h"
#include "storage/phylum_data_file.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class PhylumSuite : public ::testing::Test {
protected:
};

class RecordFaker {
public:
    fk_data_DataRecord record = fk_data_DataRecord_init_default;

public:
    void log_message(uint8_t seed) {
        record = fk_data_DataRecord_init_default;
        record.has_log = true;
        record.log.uptime = 935985493 + seed;
        record.log.time = seed * 1000;
        record.log.level = (uint32_t)LogLevels::INFO;
        record.log.facility.arg = (void *)"facility";
        record.log.facility.funcs.encode = pb_encode_string;
        record.log.message.arg = (void *)"message";
        record.log.message.funcs.encode = pb_encode_string;
    }

    fk_data_DataRecord *for_decode(Pool &pool) {
        record = fk_data_DataRecord_init_default;
        record.has_log = true;
        record.log.facility.arg = (void *)&pool;
        record.log.facility.funcs.decode = pb_decode_string;
        record.log.message.arg = (void *)&pool;
        record.log.message.funcs.decode = pb_decode_string;
        return &record;
    }
};

TEST_F(PhylumSuite, Basic_Format_NoSync_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        StandardPool pool{ "phylum" };
        Phylum phylum{ data_memory, pool };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_FALSE(phylum.mount());
    }

    loginfo("sizeof(PhylumDataFile) = %zu", sizeof(PhylumDataFile));
}

TEST_F(PhylumSuite, Basic_Format_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        StandardPool pool{ "phylum" };
        Phylum phylum{ data_memory, pool };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_TRUE(phylum.sync());
    }

    {
        StandardPool pool{ "phylum" };
        Phylum phylum{ data_memory, pool };
        ASSERT_TRUE(phylum.mount());
    }
}

TEST_F(PhylumSuite, Basic_DataFile_Create) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);
    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendAlways) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, nullptr, pool).bytes, 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, nullptr, pool).bytes, 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, nullptr, pool).bytes, 30);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendImmutable) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 0);

    fake.record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 37);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendImmutable_SeekRecordType) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 30);

    file_size_t position = 0;
    ASSERT_FALSE(file.seek_record_type(RecordType::Modules, position));

    ASSERT_EQ(file.append_immutable(RecordType::Modules, fk_data_DataRecord_fields, &fake.record, pool).bytes, 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 0);

    fake.record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool).bytes, 37);

    ASSERT_TRUE(phylum.sync());

    loginfo("sizeof(PhylumDataFile) = %zu", sizeof(PhylumDataFile));
}

TEST_F(PhylumSuite, Basic_DataFile_Reading_SeekBeginningAndEnd) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);

    auto total_written = 0u;

    RecordFaker fake;
    for (auto i = 0; i < 100; ++i) {
        StandardPool loop{ "tests" };

        fake.log_message(i);

        auto appended = file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, nullptr, loop);
        ASSERT_GT(appended.bytes, 0);

        total_written += appended.bytes;
    }

    ASSERT_EQ(file.open("d/00000000", pool), 0);

    ASSERT_EQ(file.seek_position(0), 0);

    ASSERT_EQ(file.seek_position(UINT32_MAX), (int32_t)total_written);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_Reading_SeekRecords) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    StandardPool pool{ "tests" };
    Phylum phylum{ data_memory, pool };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, pool };
    ASSERT_EQ(file.create("d/00000000", pool), 0);
    ASSERT_EQ(file.open("d/00000000", pool), 0);

    auto total_written = 0u;
    auto nrecords = 100;

    for (auto i = 0; i < nrecords; ++i) {
        StandardPool loop{ "loop" };

        RecordFaker fake;
        fake.log_message(i);

        auto appended = file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, nullptr, loop);
        ASSERT_GT(appended.bytes, 0);

        total_written += appended.bytes;
    }

    ASSERT_TRUE(phylum.sync());

    auto total_read = 0u;

    for (auto i = 0; i < nrecords; ++i) {
        StandardPool loop{ "loop" };
        PhylumDataFile file{ phylum, loop };
        ASSERT_EQ(file.open("d/00000000", loop), 0);
        ASSERT_GE(file.seek_record(i), 0);

        RecordFaker fake;
        auto bytes_read = file.read(fk_data_DataRecord_fields, fake.for_decode(pool), pool);
        ASSERT_GT(bytes_read, 0);

        ASSERT_EQ(file.close(), 0);

        total_read += bytes_read;
    }

    ASSERT_EQ(total_read, total_written);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_StartStop) {
    ASSERT_TRUE(MemoryFactory::get_data_memory()->begin());

    StandardPool pool{ "pool" };

    Storage storage{ MemoryFactory::get_data_memory(), pool, false };

    // Old fs will fail to mount even after a clear.
    ASSERT_TRUE(storage.clear());

    ASSERT_FALSE(storage.begin());

    auto ops = storage.data_ops();

    DataRecord record{ pool };
    ASSERT_FALSE(ops->read_fixed_record(record, pool));
}
