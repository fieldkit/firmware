#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/record_appender.h"
#include "storage/partitioned_reader.h"
#include "utilities.h"

using namespace fk;

FK_DECLARE_LOGGER("lfs");

class LfsSuite : public ::testing::Test {
protected:
    void SetUp() override {
        log_configure_level(LogLevels::INFO);
    }

    void TearDown() override {
        log_configure_level(LogLevels::NONE);
    }

};

TEST_F(LfsSuite, CreateAndAppendDataRecords) {
    LinuxDataMemory memory{ 20 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver{ &memory, pool };
    ASSERT_TRUE(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024 * 100, pool };

    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; i < 10; ++i) {
        StandardPool iter{ "iter" };

        auto appended = appender.append_data_record(&readings.record, iter);
        ASSERT_TRUE(appended);
        ASSERT_EQ(appended->record, i);
    }

    lfs_unmount(lfs);
}

TEST_F(LfsSuite, AppendChanges) {
    LinuxDataMemory memory{ 20 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver{ &memory, pool };
    ASSERT_TRUE(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024, pool };

    ReadingRecord readings1{ 0, 0 };
    ReadingRecord readings2{ 0, 0 };

    auto appended1 = appender.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings1.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended1);

    auto appended2 = appender.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings2.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended2);
    ASSERT_NE(appended1->record, appended2->record);
    ASSERT_NE(appended1->absolute_position, appended2->absolute_position);
    ASSERT_NE(appended1->file_position, appended2->file_position);

    auto appended3 = appender.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings2.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended3);
    ASSERT_EQ(appended3->record, appended2->record);
    ASSERT_EQ(appended3->absolute_position, appended2->absolute_position);
    ASSERT_EQ(appended3->file_position, appended2->file_position);

    lfs_unmount(lfs);
}

TEST_F(LfsSuite, ReadAcrossPartitionedFiles) {
    LinuxDataMemory memory{ 20 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver{ &memory, pool };
    ASSERT_TRUE(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024, pool };

    auto total_written = 0u;

    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; i < 4 * 20; ++i) {
        StandardPool iter{ "iter" };

        auto appended = appender.append_data_record(&readings.record, iter);

        ASSERT_TRUE(appended);
        ASSERT_EQ(appended->record, i);

        total_written += appended->record_size;
    }

    PartitionedReader reader{ &lfs_driver, &map, pool };

    // log_configure_level(LogLevels::INFO);

    auto seek1 = reader.seek(17, pool);
    ASSERT_TRUE(seek1);
    ASSERT_EQ(seek1->record, 17u);
    ASSERT_EQ(seek1->first_record_of_containing_file, 10u);
    ASSERT_EQ(seek1->absolute_position, 1902u);
    ASSERT_EQ(seek1->file_position, 784u);

    auto file_reader = reader.open_reader(pool);
    ASSERT_TRUE(file_reader);

    auto total_read = 0u;
    auto buffer = pool.malloc(1024);
    while (true) {
        auto nread = file_reader->read((uint8_t *)buffer, 1024);
        if (nread <= 0) {
            break;
        }

        total_read += nread;
    }

    ASSERT_EQ(total_written, total_read + seek1->absolute_position);

    lfs_unmount(lfs);
}
