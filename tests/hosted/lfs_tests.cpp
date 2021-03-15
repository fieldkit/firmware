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

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(&memory, true, pool));

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

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(&memory, true, pool));

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

TEST_F(LfsSuite, AppendChangesSeparateInstances) {
    LinuxDataMemory memory{ 20 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(&memory, true, pool));

    auto lfs = lfs_driver.lfs();

    ReadingRecord readings1{ 0, 0 };
    ReadingRecord readings2{ 0, 0 };

    FileMap map1{ &lfs_driver, "data", 5, pool };
    RecordAppender appender1{ &lfs_driver, &map1, 1024, pool };

    auto appended1 = appender1.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings1.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended1);

    FileMap map2{ &lfs_driver, "data", 5, pool };
    RecordAppender appender2{ &lfs_driver, &map2, 1024, pool };

    auto appended2 = appender2.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings2.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended2);
    ASSERT_NE(appended1->record, appended2->record);
    ASSERT_NE(appended1->file_position, appended2->file_position);
    ASSERT_NE(appended1->absolute_position, appended2->absolute_position);

    FileMap map3{ &lfs_driver, "data", 5, pool };
    RecordAppender appender3{ &lfs_driver, &map3, 1024, pool };

    auto appended3 = appender3.append_changes(LFS_DRIVER_FILE_ATTR_CONFIG_MODULES, &readings2.record, fk_data_DataRecord_fields, pool);
    ASSERT_TRUE(appended3);
    ASSERT_EQ(appended3->record, appended2->record);
    ASSERT_EQ(appended3->file_position, appended2->file_position);
    ASSERT_EQ(appended3->absolute_position, appended2->absolute_position);

    lfs_unmount(lfs);
}

TEST_F(LfsSuite, ReadAcrossPartitionedFiles) {
    LinuxDataMemory memory{ 20 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(&memory, true, pool));

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

TEST_F(LfsSuite, DISABLED_BlockConsumption) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "lfs" };

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(&memory, true, pool));

    auto lfs = lfs_driver.lfs();

    FileMap map{ &lfs_driver, "data", 3, pool };
    RecordAppender appender{ &lfs_driver, &map, 64 * 2048 * 5, pool };

    auto total_written = 0u;
    auto bytes_written_since_block_changed = 0u;
    auto blocks_used = 0;
    auto file_block = 0u;

    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; /*i < 100*/; ++i) {
        StandardPool iter{ "iter" };

        auto appended = appender.append_data_record(&readings.record, iter);

        ASSERT_TRUE(appended);

        total_written += appended->record_size;
        bytes_written_since_block_changed += appended->record_size;

        auto size = lfs_fs_size(lfs);
        if (size != blocks_used) {
            printf("blocks-changed records=%d blocks-used=%d bytes-written-since=%d nfiles=%d\n",
                   i, blocks_used, bytes_written_since_block_changed, map.number_files());
            bytes_written_since_block_changed = 0;
            blocks_used = size;
        }
        if (file_block != appended->first_record_of_containing_file) {
            printf("files-changed records=%d blocks-used=%d bytes-written-since=%d nfiles=%d\n",
                   i, blocks_used, bytes_written_since_block_changed, map.number_files());
            file_block = appended->first_record_of_containing_file;
        }

        if (!map.prune()) {
            logerror("prune failed");
        }
    }

    log_configure_level(LogLevels::INFO);

    lfs_unmount(lfs);
}
