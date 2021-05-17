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

TEST_F(PhylumSuite, Basic_Format_NoSync_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        Phylum phylum{ data_memory };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_FALSE(phylum.mount());
    }
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

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_log = true;
    record.log.uptime = 935985493;
    record.log.time = 0;
    record.log.level = (uint32_t)LogLevels::INFO;
    record.log.facility.arg = (void *)"facility";
    record.log.facility.funcs.encode = pb_encode_string;
    record.log.message.arg = (void *)"message";
    record.log.message.funcs.encode = pb_encode_string;

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &record, pool), 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &record, pool), 30);

    ASSERT_EQ(file.append_always(RecordType::Data, fk_data_DataRecord_fields, &record, pool), 30);

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

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_log = true;
    record.log.uptime = 935985493;
    record.log.time = 0;
    record.log.level = (uint32_t)LogLevels::INFO;
    record.log.facility.arg = (void *)"facility";
    record.log.facility.funcs.encode = pb_encode_string;
    record.log.message.arg = (void *)"message";
    record.log.message.funcs.encode = pb_encode_string;

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 0);

    record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 37);

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

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.has_log = true;
    record.log.uptime = 935985493;
    record.log.time = 0;
    record.log.level = (uint32_t)LogLevels::INFO;
    record.log.facility.arg = (void *)"facility";
    record.log.facility.funcs.encode = pb_encode_string;
    record.log.message.arg = (void *)"message";
    record.log.message.funcs.encode = pb_encode_string;

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 30);

    ASSERT_EQ(file.seek_record_type(RecordType::Modules, pool), -1);

    ASSERT_EQ(file.append_immutable(RecordType::Modules, fk_data_DataRecord_fields, &record, pool), 30);

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 0);

    record.log.message.arg = (void *)"something else";

    ASSERT_EQ(file.append_immutable(RecordType::State, fk_data_DataRecord_fields, &record, pool), 37);

    ASSERT_EQ(file.seek_record_type(RecordType::Modules, pool), 30);

    ASSERT_TRUE(phylum.sync());

    loginfo("sizeof(PhylumDataFile) = %zu", sizeof(PhylumDataFile));
}
