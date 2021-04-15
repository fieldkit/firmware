#include <directory_chain.h>
#include <directory_tree.h>
#include <free_sectors_chain.h>
#include <data_chain.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

TEST(General, EntrySizes) {
    EXPECT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 8>), 104u);
    EXPECT_EQ(sizeof(super_block_t), 9u);
    EXPECT_EQ(sizeof(directory_chain_header_t), 9u);
    EXPECT_EQ(sizeof(data_chain_header_t), 11u);
    EXPECT_EQ(sizeof(file_data_t), 29u);
    EXPECT_EQ(sizeof(file_attribute_t), 7u);
    EXPECT_EQ(sizeof(file_entry_t), 71u);
    EXPECT_EQ(sizeof(dirtree_entry_t), 67u);
    EXPECT_EQ(sizeof(dirtree_dir_t), 75u);
    EXPECT_EQ(sizeof(dirtree_file_t), 103u);
    EXPECT_EQ(sizeof(entry_t), 1u);
    EXPECT_EQ(sizeof(tree_node_header_t), 16u);
}

template <typename T> class BasicsFixture : public PhylumFixture {};

typedef ::testing::Types<
    std::pair<layout_256, directory_chain>,
    std::pair<layout_4096, directory_chain>,
    std::pair<layout_4096, directory_tree>
    >
    Implementations;

TYPED_TEST_SUITE(BasicsFixture, Implementations);

TYPED_TEST(BasicsFixture, MountFormatMount) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.begin(true);

    dir_type dir{ memory.buffers(), memory.sectors(), memory.allocator(), 0 };
    ASSERT_EQ(dir.mount(), -1);
    ASSERT_EQ(dir.format(), 0);
    ASSERT_EQ(dir.mount(), 0);
}

TYPED_TEST(BasicsFixture, FormatPersists) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.begin(true);

    memory.sync([&]() {
        dir_type dir{ memory.buffers(), memory.sectors(), memory.allocator(), 0 };
        ASSERT_EQ(dir.format(), 0);
    });

    memory.sync([&]() {
        dir_type dir{ memory.buffers(), memory.sectors(), memory.allocator(), 0 };
        ASSERT_EQ(dir.mount(), 0);
    });
}

TYPED_TEST(BasicsFixture, FindAndTouch) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("other.logs", open_file_config{}), 0);
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 0);
        ASSERT_EQ(dir.touch("test.logs"), 0);
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 1);
        ASSERT_EQ(dir.find("other.logs", open_file_config{}), 0);
    });
}

TYPED_TEST(BasicsFixture, FindAndTouchAndUnlink) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("other.logs", open_file_config{}), 0);
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 0);
        ASSERT_EQ(dir.touch("test.logs"), 0);
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 1);
        ASSERT_EQ(dir.find("other.logs", open_file_config{}), 0);
        ASSERT_EQ(dir.unlink("test.logs"), 0);
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 0);
    });
}

TYPED_TEST(BasicsFixture, TouchPersists) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("test.logs"), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("test.logs", open_file_config{}), 1);
    });
}

TYPED_TEST(BasicsFixture, TouchAndFindMultiple) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    const char *names[] = {
        "a.txt",
        "b.txt",
        "c.txt",
        "d.txt",
        "e.txt",
        "f.txt",
        "g.txt"
    };
    memory.mounted<dir_type>([&](auto &dir) {
        for (auto name : names) {
            ASSERT_EQ(dir.touch(name), 0);
        }
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("nope.txt", open_file_config{}), 0);
        for (auto name : names) {
            ASSERT_EQ(dir.find(name, open_file_config{}), 1);
        }
        ASSERT_EQ(dir.find("nope.txt", open_file_config{}), 0);
    });
}

TYPED_TEST(BasicsFixture, TouchAndFindAndUnlinkMultiple) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    const char *names[] = {
        "a.txt",
        "b.txt",
        "c.txt",
        "d.txt",
        "e.txt",
        "f.txt",
        "g.txt"
    };
    memory.mounted<dir_type>([&](auto &dir) {
        for (auto name : names) {
            ASSERT_EQ(dir.touch(name), 0);
        }
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("nope.txt", open_file_config{}), 0);
        for (auto name : names) {
            ASSERT_EQ(dir.find(name, open_file_config{}), 1);
        }
        ASSERT_EQ(dir.find("nope.txt", open_file_config{}), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        for (auto name : names) {
            ASSERT_EQ(dir.unlink(name), 0);
        }
    });

    memory.mounted<dir_type>([&](auto &dir) {
        for (auto name : names) {
            ASSERT_EQ(dir.find(name, open_file_config{}), 0);
        }
    });
}

TYPED_TEST(BasicsFixture, FreeSectorsChain_Empty) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        data_chain dc{ memory.buffers(), memory.sectors(), memory.allocator(), head_tail_t{ }, "dc" };
        ASSERT_EQ(dc.create_if_necessary(), 0);

        free_sectors_chain fsc{ memory.buffers(), memory.sectors(), memory.allocator(), head_tail_t{ }, "fsc" };
        ASSERT_EQ(fsc.create_if_necessary(), 0);

        dhara_sector_t sector = 0;
        ASSERT_EQ(fsc.dequeue(&sector), 0);

        ASSERT_EQ(fsc.add_chain(dc.head()), 0);
    });
}
