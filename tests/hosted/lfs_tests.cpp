#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "storage/block_appender.h"
#include "storage/partitioned_reader.h"
#include "utilities.h"

using namespace fk;

FK_DECLARE_LOGGER("lfs");

class LfsSuite : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
        log_configure_level(LogLevels::NONE);
    }

};

TEST_F(LfsSuite, Create) {
    StandardPool pool{ "lfs" };
    auto memory = MemoryFactory::get_data_memory();

    ASSERT_TRUE(memory->begin());

    LfsDriver lfs_driver{ memory, pool };
    ASSERT_TRUE(lfs_driver.begin(true));

    auto lfs = lfs_driver.lfs();

    // log_configure_level(LogLevels::INFO);

    FileMap map{ &lfs_driver, "data", pool };
    BlockAppender appender{ &lfs_driver, &map, 1024, pool };

    ReadingRecord readings{ 0, 0 };
    for (auto i = 0u; i < 4 * 20; ++i) {
        StandardPool iter{ "iter" };

        auto appended = appender.append_always(&readings.record, iter);
        ASSERT_TRUE(appended);
        ASSERT_EQ(appended->block, i);
    }

    PartitionedReader reader{ &lfs_driver, &map, pool };

    auto seek1 = reader.seek(17, pool);
    ASSERT_TRUE(seek1);
    ASSERT_EQ(seek1->block, 17u);
    ASSERT_EQ(seek1->first_block_of_containing_file, 10u);
    ASSERT_EQ(seek1->absolute_position, 2970u);
    ASSERT_EQ(seek1->file_position, 770u);

    auto seek2 = reader.seek(64, pool);
    ASSERT_TRUE(seek2);
    ASSERT_EQ(seek2->block, 64u);
    ASSERT_EQ(seek2->first_block_of_containing_file, 60u);
    ASSERT_EQ(seek2->absolute_position, 8140u);
    ASSERT_EQ(seek2->file_position, 440u);

    lfs_unmount(lfs);
}
