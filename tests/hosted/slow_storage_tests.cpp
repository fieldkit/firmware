#include <fk-data-protocol.h>

#include "storage_suite.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class SlowStorageSuite : public StorageSuite {
};

TEST_F(SlowStorageSuite, LargeFiles) {
    Storage storage{ memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file_write = storage.file(0);
    size_t size = 10 * 1024 * 1024;

    ASSERT_TRUE(file_write.create());

    StaticPattern pattern;
    pattern.write(file_write, size);

    ASSERT_EQ(file_write.size(), size);
    ASSERT_EQ(file_write.position(), size);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file_read = storage.file(0);

    ASSERT_TRUE(file_read.seek_end());
    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), (size_t)size);

    pattern.verify(file_read, size);

    ASSERT_EQ(file_read.size(), size);
    ASSERT_EQ(file_read.position(), size);
}

TEST_F(SlowStorageSuite, LargeFilesMultiple) {
    Storage storage{ memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);
    auto size = 10 * 1024 * 1024;

    ASSERT_TRUE(file0_write.create());
    ASSERT_TRUE(file1_write.create());

    ASSERT_EQ(file0_write.position(), (size_t)0);
    ASSERT_EQ(file0_write.size(), (size_t)0);
    ASSERT_EQ(file1_write.position(), (size_t)0);
    ASSERT_EQ(file1_write.size(), (size_t)0);

    StaticPattern pattern;

    auto counter = 0;
    auto bytes_wrote = 0;
    while (bytes_wrote < size) {
        ASSERT_EQ(file0_write.write(pattern.data, sizeof(pattern.data)), (int32_t)sizeof(pattern.data));
        ASSERT_EQ(file1_write.write(pattern.data, sizeof(pattern.data)), (int32_t)sizeof(pattern.data));

        bytes_wrote += sizeof(pattern.data);

        counter++;

        ASSERT_EQ(file0_write.position(), sizeof(pattern.data) * counter);
        ASSERT_EQ(file1_write.position(), sizeof(pattern.data) * counter);
    }

    ASSERT_EQ(file0_write.position(), (size_t)size);
    ASSERT_EQ(file0_write.size(), (size_t)size);
    ASSERT_EQ(file1_write.position(), (size_t)size);
    ASSERT_EQ(file1_write.size(), (size_t)size);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    ASSERT_TRUE(file0_read.seek_end());
    ASSERT_EQ(file0_read.position(), (size_t)size);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    pattern.verify(file0_read, size);

    ASSERT_EQ(file0_read.position(), (size_t)size);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    auto file1_read = storage.file(1);

    ASSERT_TRUE(file1_read.seek_end());
    ASSERT_EQ(file1_read.position(), (size_t)size);
    ASSERT_EQ(file1_read.size(), (size_t)size);

    pattern.verify(file1_read, size);

    ASSERT_EQ(file1_read.position(), (size_t)size);

    NoopProgressCallbacks progress;
    FK_ASSERT(storage.fsck(&progress));
}

TEST_F(SlowStorageSuite, LargeFilesMultipleOneMuchSmaller) {
    Storage storage{ memory_, false };

    ASSERT_TRUE(storage.clear());

    auto file0_write = storage.file(0);
    auto file1_write = storage.file(1);
    auto size = 10 * 1024 * 1024;

    ASSERT_TRUE(file0_write.create());
    ASSERT_TRUE(file1_write.create());

    StaticPattern pattern;

    auto bytes_wrote = 0;
    while (bytes_wrote < size) {
        ASSERT_EQ(file0_write.write(pattern.data, sizeof(pattern.data)), (int32_t)sizeof(pattern.data));

        if (bytes_wrote % 4096 == 0) {
            ASSERT_EQ(file1_write.write(pattern.data, sizeof(pattern.data)), (int32_t)sizeof(pattern.data));
        }

        bytes_wrote += sizeof(pattern.data);
    }

    auto smaller_size = (size_t)(size / 4096) * sizeof(pattern.data);

    ASSERT_EQ(file0_write.position(), (size_t)size);
    ASSERT_EQ(file0_write.size(), (size_t)size);
    ASSERT_EQ(file1_write.position(), smaller_size);
    ASSERT_EQ(file1_write.size(), smaller_size);

    ASSERT_TRUE(memory_->flush());

    ASSERT_TRUE(storage.begin());

    auto file0_read = storage.file(0);

    ASSERT_TRUE(file0_read.seek_end());
    ASSERT_EQ(file0_read.position(), (size_t)size);
    ASSERT_EQ(file0_read.size(), (size_t)size);

    pattern.verify(file0_read, size);

    auto file1_read = storage.file(1);

    ASSERT_TRUE(file1_read.seek_end());
    ASSERT_EQ(file1_read.position(), (size_t)smaller_size);
    ASSERT_EQ(file1_read.size(), smaller_size);

    pattern.verify(file1_read, smaller_size);

    ASSERT_EQ(file1_read.position(), smaller_size);

    NoopProgressCallbacks progress;
    FK_ASSERT(storage.fsck(&progress));
}

TEST_F(SlowStorageSuite, LotsOfIndividualWrites) {
    uint32_t total_wrote = 0;

    {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        auto wrote = write_reading(file_write);
        FK_ASSERT(wrote > 0);
        total_wrote += wrote;
    }

    for (auto i = 0; i < 1024; ++i) {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.begin());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.seek_end());
        auto wrote = write_reading(file_write);
        FK_ASSERT(wrote > 0);
        total_wrote += wrote;
    }

    for (auto i = 0; i < 1024; ++i) {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.begin());
        auto file_write = storage.file(1);
        ASSERT_TRUE(file_write.create());
        auto wrote = write_reading(file_write);
        FK_ASSERT(wrote > 0);
    }

    {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.begin());
        auto file_read = storage.file(0);

        ASSERT_TRUE(file_read.seek_end());
        ASSERT_EQ(file_read.position(), total_wrote);
        ASSERT_EQ(file_read.size(), total_wrote);

        ASSERT_TRUE(file_read.seek(0));
        ASSERT_EQ(file_read.position(), (size_t)0);

        NoopProgressCallbacks progress;
        FK_ASSERT(storage.fsck(&progress));
    }
}

TEST_F(SlowStorageSuite, ErasingLargeFsAndStartingOver) {
    uint32_t version1;
    uint32_t version2;

    // Write a ton of data, spanning multiple blocks.
    {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        auto bytes_to_write = memory_->geometry().block_size * 50;
        auto total_wrote = (size_t)0;
        while (total_wrote < bytes_to_write) {
            auto wrote = write_reading(file_write);
            FK_ASSERT(wrote > 0);
            total_wrote += wrote;
        }
        version1 = storage.version();
    }

    loginfo("version #1 = %" PRIu32, version1);

    // Now start over and write a little data.
    {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.clear());
        auto file_write = storage.file(0);
        ASSERT_TRUE(file_write.create());
        auto bytes_to_write = memory_->geometry().block_size * 5;
        auto total_wrote = (size_t)0;
        while (total_wrote < bytes_to_write) {
            auto wrote = write_reading(file_write);
            FK_ASSERT(wrote > 0);
            total_wrote += wrote;
        }
        version2 = storage.version();
        FK_ASSERT(version2 != version1);
    }

    loginfo("version #2 = %" PRIu32, version2);

    // Now we open.
    {
        Storage storage{ memory_, false };
        ASSERT_TRUE(storage.begin());
        FK_ASSERT(storage.version() == version2);
    }
}
