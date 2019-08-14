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
#include "scanning.h"
#include "modules_bridge.h"
#include "registry.h"
#include "meta.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class MetaLogSuite : public ::testing::Test {
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
    }

protected:
    void enable_debug() {
        log_configure_level(LogLevels::DEBUG);
    }

    void enable_trace() {
        log_configure_level(LogLevels::TRACE);
    }

    void enable_verbose() {
        log_configure_level(LogLevels::VERBOSE);
    }

};

TEST_F(MetaLogSuite, OpeningEmptyFile) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    // No entry for this yet, so this will fail.
    ASSERT_FALSE(srl.seek_record(SignedRecordKind::Modules));
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

static void append_other(SignedRecordLog &srl, const char *build, const char *git, Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.time = 1;
    record.metadata.git.funcs.encode = pb_encode_string;
    record.metadata.git.arg = (void *)git;
    record.metadata.build.funcs.encode = pb_encode_string;
    record.metadata.build.arg = (void *)build;
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Other, &record, fk_data_DataRecord_fields, pool));
}

TEST_F(MetaLogSuite, AppendingAnEntry) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

    auto file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ file };

    // No entry for this yet, so this will fail.
    ASSERT_FALSE(srl.seek_record(SignedRecordKind::Modules));

    append_metadata_always(srl, 1, "our-build-1", "our-git-1", pool);
}

TEST_F(MetaLogSuite, AppendingTwoLogs) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

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

TEST_F(MetaLogSuite, AppendingImmutable) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

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

TEST_F(MetaLogSuite, AppendingImmutableWithOtherKindsBetween) {
    StaticPool<1024> pool{ "meta-log" };
    GlobalState gs;
    Storage storage{ data_memory_ };
    MetaLog meta_log{ &storage, &gs };
    ResolvedModules resolved;

    ASSERT_TRUE(storage.clear());
    ASSERT_TRUE(meta_log.open());

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
