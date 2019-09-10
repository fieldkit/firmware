#include <fk-data-protocol.h>

#include "tests.h"
#include "patterns.h"
#include "test_modules.h"

#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "clock.h"
#include "protobuf.h"
#include "modules/scanning.h"
#include "modules/registry.h"
#include "modules/bridge/modules_bridge.h"
#include "state.h"
#include "storage_suite.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class SignedLogSuite : public StorageSuite {
};

static void append_metadata_always(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, Pool &pool);
static void append_metadata(SignedRecordLog &srl, const char *build, const char *git, Pool &pool);
static void append_other_always(SignedRecordLog &srl, const char *build, const char *git, Pool &pool);
/*
static void append_other(SignedRecordLog &srl, const char *build, const char *git, Pool &pool);
*/

TEST_F(SignedLogSuite, OpeningEmptyFile) {
    StaticPool<1024> pool{ "signed-log" };
    GlobalState gs;
    Storage storage{ data_memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    // No entry for this yet, so this will fail.
    ASSERT_FALSE(srl.seek_record(SignedRecordKind::Modules));
}

TEST_F(SignedLogSuite, AppendingAnEntry) {
    StaticPool<1024> pool{ "signed-log" };
    GlobalState gs;
    Storage storage{ data_memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    // No entry for this yet, so this will fail.
    ASSERT_FALSE(srl.seek_record(SignedRecordKind::Modules));

    append_metadata_always(srl, 1, "our-build-1", "our-git-1", pool);

    ASSERT_TRUE(srl.seek_record(SignedRecordKind::Modules));

    auto file_read = storage.file(Storage::Meta);
    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.record(), (uint32_t)1);

    Storage storage_2{ data_memory_, false };
    ASSERT_TRUE(storage_2.begin());
    auto file_read_2 = storage_2.file(Storage::Meta);
    ASSERT_TRUE(file_read_2.seek_end());
    ASSERT_EQ(file_read_2.record(), (uint32_t)1);
}

TEST_F(SignedLogSuite, AppendingTwoLogs) {
    StaticPool<1024> pool{ "signed-log" };
    GlobalState gs;
    Storage storage{ data_memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    // No entry for this yet, so this will fail.
    ASSERT_FALSE(srl.seek_record(SignedRecordKind::Modules));

    append_metadata_always(srl, 1, "our-build-1", "our-git-1", pool);

    ASSERT_TRUE(srl.seek_record(SignedRecordKind::Modules));

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    ASSERT_TRUE(srl.decode(&record, fk_data_DataRecord_fields, pool));
    ASSERT_EQ(record.metadata.time, (uint32_t)1);

    ASSERT_TRUE(srl.seek_end());
    append_metadata_always(srl, 2, "our-build-2", "our-git-2", pool);

    ASSERT_TRUE(srl.seek_record(SignedRecordKind::Modules));

    record = fk_data_DataRecord_init_default;
    ASSERT_TRUE(srl.decode(&record, fk_data_DataRecord_fields, pool));
    ASSERT_EQ(record.metadata.time, (uint32_t)2);
}

TEST_F(SignedLogSuite, AppendingImmutable) {
    StaticPool<1024> pool{ "signed-log" };
    GlobalState gs;
    Storage storage{ data_memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    append_metadata(srl, "our-build-1", "our-git-1", pool);

    auto position1 = file.position();
    ASSERT_GT(position1, (uint32_t)0);

    append_metadata(srl, "our-build-1", "our-git-1", pool);

    auto position2 = file.position();
    ASSERT_EQ(position1, position2);

    append_metadata(srl, "our-build-2", "our-git-2", pool);

    auto position3 = file.position();
    ASSERT_GT(position3, position2);
}

TEST_F(SignedLogSuite, AppendingImmutableWithOtherKindsBetween) {
    StaticPool<1024> pool{ "signed-log" };
    GlobalState gs;
    Storage storage{ data_memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    append_metadata(srl, "our-build-1", "our-git-1", pool);

    append_other_always(srl, "ignored", "ignored", pool);
    append_other_always(srl, "ignored", "ignored", pool);

    auto position1 = file.position();
    ASSERT_GT(position1, (uint32_t)0);

    append_metadata(srl, "our-build-1", "our-git-1", pool);

    auto position2 = file.position();
    ASSERT_EQ(position1, position2);

    append_other_always(srl, "ignored", "ignored", pool);
    append_other_always(srl, "ignored", "ignored", pool);
    append_other_always(srl, "ignored", "ignored", pool);

    append_metadata(srl, "our-build-2", "our-git-2", pool);

    auto position3 = file.position();
    ASSERT_GT(position3, position2);

    append_other_always(srl, "ignored", "ignored", pool);
    append_other_always(srl, "ignored", "ignored", pool);
}

static void append_metadata_always(SignedRecordLog &srl, uint32_t time, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = time;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

static void append_metadata(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool));
}

static void append_other_always(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_always(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

/*
static void append_other(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}
*/
