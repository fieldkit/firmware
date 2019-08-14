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

    fk_data_DataRecord record1 = fk_data_DataRecord_init_default;
    record1.metadata.time = 1;
    record1.metadata.git.funcs.encode = pb_encode_string;
    record1.metadata.git.arg = (void *)"our-git-1";
    record1.metadata.build.funcs.encode = pb_encode_string;
    record1.metadata.build.arg = (void *)"our-build-1";

    ASSERT_TRUE(srl.append(SignedRecordKind::Modules, &record1, fk_data_DataRecord_fields, pool));
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

    fk_data_DataRecord record1 = fk_data_DataRecord_init_default;
    record1.metadata.time = 1;
    record1.metadata.git.funcs.encode = pb_encode_string;
    record1.metadata.git.arg = (void *)"our-git-1";
    record1.metadata.build.funcs.encode = pb_encode_string;
    record1.metadata.build.arg = (void *)"our-build-1";

    ASSERT_TRUE(srl.append(SignedRecordKind::Modules, &record1, fk_data_DataRecord_fields, pool));

    ASSERT_TRUE(srl.seek_record(SignedRecordKind::Modules));

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    ASSERT_TRUE(srl.decode(&record, fk_data_DataRecord_fields, pool));
    ASSERT_EQ(record.metadata.time, (uint32_t)1);

    fk_data_DataRecord record2 = fk_data_DataRecord_init_default;
    record2.metadata.time = 2;
    record2.metadata.git.funcs.encode = pb_encode_string;
    record2.metadata.git.arg = (void *)"our-git-2";
    record2.metadata.build.funcs.encode = pb_encode_string;
    record2.metadata.build.arg = (void *)"our-build-2";
    ASSERT_TRUE(srl.seek_end());
    ASSERT_TRUE(srl.append(SignedRecordKind::Modules, &record2, fk_data_DataRecord_fields, pool));

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

    fk_data_DataRecord record1 = fk_data_DataRecord_init_default;
    record1.metadata.time = 1;
    record1.metadata.git.funcs.encode = pb_encode_string;
    record1.metadata.git.arg = (void *)"our-git-1";
    record1.metadata.build.funcs.encode = pb_encode_string;
    record1.metadata.build.arg = (void *)"our-build-1";
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record1, fk_data_DataRecord_fields, pool));

    auto position1 = file.position();
    ASSERT_GT(position1, (uint32_t)0);

    fk_data_DataRecord record2 = fk_data_DataRecord_init_default;
    record2.metadata.time = 1;
    record2.metadata.git.funcs.encode = pb_encode_string;
    record2.metadata.git.arg = (void *)"our-git-1";
    record2.metadata.build.funcs.encode = pb_encode_string;
    record2.metadata.build.arg = (void *)"our-build-1";
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record2, fk_data_DataRecord_fields, pool));

    auto position2 = file.position();
    ASSERT_EQ(position1, position2);

    fk_data_DataRecord record3 = fk_data_DataRecord_init_default;
    record3.metadata.time = 1;
    record3.metadata.git.funcs.encode = pb_encode_string;
    record3.metadata.git.arg = (void *)"our-git-1";
    record3.metadata.build.funcs.encode = pb_encode_string;
    record3.metadata.build.arg = (void *)"our-build-2";
    ASSERT_TRUE(srl.append_immutable(SignedRecordKind::Modules, &record3, fk_data_DataRecord_fields, pool));

    auto position3 = file.position();
    ASSERT_GT(position3, position2);
}
