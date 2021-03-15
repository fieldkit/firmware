#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/dhara.h"
#include "storage/lfs_driver.h"
#include "storage/record_appender.h"
#include "storage/partitioned_reader.h"
#include "utilities.h"

using namespace fk;

FK_DECLARE_LOGGER("dhara");

class DharaSuite : public ::testing::Test {
protected:
    void SetUp() override {
        log_configure_level(LogLevels::INFO);
    }

    void TearDown() override {
        log_configure_level(LogLevels::NONE);
    }

};

TEST_F(DharaSuite, CreateAndMount) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    auto sector = (uint8_t *)pool.malloc(dhara.page_size());

    memcpy(sector, "jacob", strlen("jacob"));

    if (!dhara.write(0, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.write(1, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.sync()) {
        logerror("sync");
    }

    if (!dhara.write(1, sector, dhara.page_size())) {
        logerror("write");
    }

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsMountOnly) {
    LinuxDataMemory memory{ 1024 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    LfsDriver lfs_driver;
    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }

    loginfo("mounting lfs!");

    ASSERT_TRUE(lfs_driver.begin(dhara.map(), false, pool));

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsAppendOne) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024 * 100, pool };
    ReadingRecord readings{ 0, 0 };
    auto appended = appender.append_data_record(&readings.record, pool);
    ASSERT_TRUE(appended);

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsAppendMultiple) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    FileMap map{ &lfs_driver, "data", 5, pool };
    RecordAppender appender{ &lfs_driver, &map, 1024 * 100, pool };
    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; i < 10; ++i) {
        StandardPool iter{ "iter" };
        auto appended = appender.append_data_record(&readings.record, iter);
        ASSERT_TRUE(appended);
        ASSERT_EQ(appended->record, i);
    }

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }
}

TEST_F(DharaSuite, LfsCreateWriteRead) {
    LinuxDataMemory memory{ 100 };
    StandardPool pool{ "dhara" };

    log_configure_level(LogLevels::DEBUG);

    ASSERT_TRUE(memory.begin());

    Dhara dhara;
    ASSERT_TRUE(dhara.begin(&memory, true, pool));

    loginfo("mounting lfs!");

    LfsDriver lfs_driver;
    ASSERT_TRUE(lfs_driver.begin(dhara.map(), true, pool));

    auto lfs = lfs_driver.lfs();

    lfs_file_t file;
    uint32_t counter = 0;
    lfs_file_open(lfs, &file, "counter", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(lfs, &file, &counter, sizeof(counter));
    counter += 1;
    lfs_file_rewind(lfs, &file);
    lfs_file_write(lfs, &file, &counter, sizeof(counter));
    lfs_file_close(lfs, &file);

    loginfo("counter: %d", counter);

    lfs_unmount(lfs_driver.lfs());

    if (!dhara.sync()) {
        logerror("sync");
    }

    lfs_file_open(lfs, &file, "counter", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(lfs, &file, &counter, sizeof(counter));
    counter += 1;
    lfs_file_rewind(lfs, &file);
    lfs_file_write(lfs, &file, &counter, sizeof(counter));
    lfs_file_close(lfs, &file);

    loginfo("counter: %d", counter);

    if (!dhara.sync()) {
        logerror("sync");
    }
}
