#include "tests.h"
#include "patterns.h"

#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "clock.h"
#include "state.h"
#include "storage_suite.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class BadBlocksSuite : public StorageSuite {
};

TEST_F(BadBlocksSuite, FindingBadBlockFromFactoryDuringClear) {
    Storage storage{ memory_, pool_, false };

    bank(0).mark_block_bad_from_factory(g_.block_size * 2);

    ASSERT_TRUE(storage.clear());
}

TEST_F(BadBlocksSuite, FindingBadBlockFromWearDuringClear) {
    Storage storage{ memory_, pool_, false };

    bank(0).mark_block_bad_from_wear(g_.block_size * 2);

    ASSERT_TRUE(storage.clear());
}

TEST_F(BadBlocksSuite, FindingFactoryBadBlockAfterCleared) {
    Storage storage{ memory_, pool_, false };

    ASSERT_TRUE(storage.clear());

    bank(0).mark_block_bad_from_factory(g_.block_size * 2);

    auto file_write = storage.file(0);
    auto size = g_.block_size * 3u;

    ASSERT_TRUE(file_write.create());

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), (size_t)size);

    pattern.verify(file_read, size);

    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), size);
}

TEST_F(BadBlocksSuite, EraseFailsOnBadBlockFromWearAfterCleared) {
    Storage storage{ memory_, pool_, false };

    ASSERT_TRUE(storage.clear());

    bank(0).mark_block_bad_from_wear(g_.block_size * 2);

    auto file_write = storage.file(0);
    auto size = g_.block_size * 3u;

    ASSERT_TRUE(file_write.create());

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), (size_t)size);

    pattern.verify(file_read, size);

    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), size);
}

TEST_F(BadBlocksSuite, WritingToBadRegion) {
    bank(0).mark_region_bad(g_.block_size * 1, 2048);

    Storage storage{ memory_, pool_, false };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = g_.block_size * 2u;

    ASSERT_TRUE(file_write.create());

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());
}

TEST_F(BadBlocksSuite, WritingToBadRegionDuringFileWrite) {
    bank(0).mark_region_bad(g_.block_size * 1 + 512, 2048 - 512);

    Storage storage{ memory_, pool_, false };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    auto size = g_.block_size * 2u;

    ASSERT_TRUE(file_write.create());

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(storage.begin());
}
