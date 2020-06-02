#include "tests.h"

#include "mocks_and_fakes.h"
#include "readings_taker.h"
#include "state.h"
#include "storage_suite.h"
#include "test_modules.h"
#include "modules/module_factory.h"
#include "storage/signed_log.h"

using testing::Return;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::_;
using namespace fk;

FK_DECLARE_LOGGER("tests");

class ReadingsTakingSuite : public StorageSuite {
protected:
    void SetUp() override {
        StorageSuite::SetUp();
        fk_modules_builtin_register(&fk_test_module_fake_empty);
        fk_modules_builtin_register(&fk_test_module_fake_1);
        fk_modules_builtin_register(&fk_test_module_fake_2);
    }

    void TearDown() override {
        fk_modules_builtin_clear();
        StorageSuite::TearDown();
    }
};

TEST_F(ReadingsTakingSuite, WithNoModules) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus, pool_ };
    Storage storage{ memory_, pool_, false };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(as_expected(FoundModuleCollection(found))));
    ModuleFactory module_factory;
    auto constructed_maybe = module_factory.rescan_and_initialize(ctx, scanning, pool_);

    ReadingsTaker readings_taker{ storage, get_modmux(), false };
    ASSERT_TRUE(readings_taker.take(*constructed_maybe, ctx, pool_));
}

TEST_F(ReadingsTakingSuite, BasicSingleModule) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus, pool_ };
    Storage storage{ memory_, pool_, false };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);
    found.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(as_expected(FoundModuleCollection(found))));
    ModuleFactory module_factory;
    auto constructed_maybe = module_factory.rescan_and_initialize(ctx, scanning, pool_);

    ReadingsTaker readings_taker{ storage, get_modmux(), false };
    ASSERT_TRUE(readings_taker.take(*constructed_maybe, ctx, pool_));
}

TEST_F(ReadingsTakingSuite, BasicTwoModules) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus, pool_ };
    Storage storage{ memory_, pool_, false };
    FK_ASSERT(storage.clear());

    FoundModuleCollection found(pool_);
    found.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });
    found.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x03,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_)).WillOnce(Return(as_expected(FoundModuleCollection(found))));
    ModuleFactory module_factory;
    auto constructed_maybe = module_factory.rescan_and_initialize(ctx, scanning, pool_);

    ReadingsTaker readings_taker{ storage, get_modmux(), false };
    ASSERT_TRUE(readings_taker.take(*constructed_maybe, ctx, pool_));
}

TEST_F(ReadingsTakingSuite, AssignsRecordIndices) {
    GlobalState gs;
    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus, pool_ };
    Storage storage{ memory_, pool_, false };
    FK_ASSERT(storage.clear());

    FoundModuleCollection one_module(pool_);
    one_module.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });

    FoundModuleCollection two_modules(pool_);
    two_modules.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x02,
        }
    });
    two_modules.emplace(FoundModule{
        .position = 0xff,
        .header = {
            .manufacturer = FK_MODULES_MANUFACTURER,
            .kind = FK_MODULES_KIND_RANDOM,
            .version = 0x03,
        }
    });

    MockModuleScanning scanning;
    EXPECT_CALL(scanning, scan(_))
        .WillOnce(Return(as_expected(FoundModuleCollection(one_module))))
        .WillOnce(Return(as_expected(FoundModuleCollection(two_modules))))
        .WillOnce(Return(as_expected(FoundModuleCollection(two_modules))))
        .WillOnce(Return(as_expected(FoundModuleCollection(one_module))))
        .WillOnce(Return(as_expected(FoundModuleCollection(one_module))))
        .WillOnce(Return(as_expected(FoundModuleCollection(two_modules))));
    ReadingsTaker readings_taker{ storage, get_modmux(), false };

    loginfo("tests: appending record 1");

    ModuleFactory module_factory;
    auto constructed_maybe1 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe1, ctx, pool_));

    loginfo("tests: appending record 2");

    auto constructed_maybe2 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe2, ctx, pool_));

    loginfo("tests: appending record 3");

    auto constructed_maybe3 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe3, ctx, pool_));

    loginfo("tests: appending record 4");

    auto constructed_maybe4 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe4, ctx, pool_));

    loginfo("tests: appending record 5");

    auto constructed_maybe5 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe5, ctx, pool_));

    loginfo("tests: appending record 6");

    auto constructed_maybe6 = module_factory.rescan_and_initialize(ctx, scanning, pool_);
    ASSERT_TRUE(readings_taker.take(*constructed_maybe6, ctx, pool_));

    loginfo("tests: seeking record 4");

    auto meta_file = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ meta_file };
    ASSERT_EQ(*srl.seek_record(SignedRecordKind::Modules), (uint32_t)4);

    fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
    ASSERT_TRUE(meta_file.read(&sr, fk_data_SignedRecord_fields));
    ASSERT_EQ(sr.record, (uint32_t)4);

    auto data_file = storage.file(Storage::Data);
    ASSERT_TRUE(data_file.seek(0));
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)1);
    ASSERT_EQ(record.readings.meta, (uint32_t)1);

    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)2);
    ASSERT_EQ(record.readings.meta, (uint32_t)2);

    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)3);
    ASSERT_EQ(record.readings.meta, (uint32_t)2);

    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)4);
    ASSERT_EQ(record.readings.meta, (uint32_t)3);

    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)5);
    ASSERT_EQ(record.readings.meta, (uint32_t)3);

    ASSERT_TRUE(data_file.read(&record, fk_data_DataRecord_fields));
    ASSERT_EQ(record.readings.reading, (uint32_t)6);
    ASSERT_EQ(record.readings.meta, (uint32_t)4);
}
