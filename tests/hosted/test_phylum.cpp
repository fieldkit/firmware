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
        record.has_log = true;
        record.log.uptime = 935985493 + seed;
        record.log.time = seed * 1000;
        record.log.level = (uint32_t)LogLevels::INFO;
        record.log.facility.arg = (void *)"facility";
        record.log.facility.funcs.encode = pb_encode_string;
        record.log.message.arg = (void *)"message";
        record.log.message.funcs.encode = pb_encode_string;
    }

};

TEST_F(PhylumSuite, Basic_Format_NoSync_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        Phylum phylum{ data_memory };
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
        Phylum phylum{ data_memory };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_TRUE(phylum.sync());
    }

    {
        Phylum phylum{ data_memory };
        ASSERT_TRUE(phylum.mount());
    }
}

TEST_F(PhylumSuite, Basic_DataFile_Create) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);
    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendAlways) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendImmutable) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 0);

    fake.record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 37);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_AppendImmutable_SeekRecordType) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);

    RecordFaker fake;
    fake.log_message(0);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_EQ(file.seek_record_type(RecordType::Modules, pool), -1);

    ASSERT_EQ(file.append_immutable(RecordType::Modules, fk_data_DataRecord_fields, &fake.record, pool), 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 0);

    fake.record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &fake.record, pool), 37);

    ASSERT_TRUE(phylum.sync());

    loginfo("sizeof(PhylumDataFile) = %zu", sizeof(PhylumDataFile));
}

TEST_F(PhylumSuite, Basic_DataFile_Reading_SeekBeginningAndEnd) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);

    auto total_written = 0u;

    RecordFaker fake;
    for (auto i = 0; i < 100; ++i) {
        StandardPool loop{ "tests" };

        fake.log_message(i);

        auto wrote = file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, loop);
        ASSERT_GT(wrote, 0);

        total_written += wrote;
    }

    ASSERT_EQ(file.open(pool), 0);

    ASSERT_EQ(file.seek_position(0, pool), 0);

    ASSERT_EQ(file.seek_position(UINT32_MAX, pool), (int32_t)total_written);

    ASSERT_TRUE(phylum.sync());
}

TEST_F(PhylumSuite, Basic_DataFile_Reading_SeekRecords) {
    StandardPool pool{ "tests" };
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    Phylum phylum{ data_memory };
    ASSERT_TRUE(phylum.format());
    PhylumDataFile file{ phylum, "d/00000000", pool };
    ASSERT_EQ(file.create(pool), 0);
    ASSERT_EQ(file.open(pool), 0);

    auto total_written = 0u;

    for (auto i = 0; i < 100; ++i) {
        StandardPool loop{ "loop" };

        RecordFaker fake;
        fake.log_message(i);

        auto wrote = file.append_always(RecordType::Data, fk_data_DataRecord_fields, &fake.record, loop);
        ASSERT_GT(wrote, 0);

        total_written += wrote;
    }

    ASSERT_TRUE(phylum.sync());

    for (auto i = 0; i < 100; ++i) {
        StandardPool loop{ "loop" };
        PhylumDataFile file{ phylum, "d/00000000", loop };
        ASSERT_EQ(file.open(loop), 0);
        ASSERT_GE(file.seek_record(i, loop), 0);
        ASSERT_EQ(file.close(), 0);
    }

    ASSERT_TRUE(phylum.sync());
}
