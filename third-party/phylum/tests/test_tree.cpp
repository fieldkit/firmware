#include <directory_chain.h>
#include <file_appender.h>
#include <tree_sector.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

TEST(TreeInfo, NodeSizes) {
    EXPECT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 6>), 84u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 6>), 112u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint64_t, 6>), 112u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 64>), 920u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 128>), 1816u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 288>), 4056u);
    EXPECT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 291>), 4096u);
    EXPECT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 291>), 2932u);
    EXPECT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 407>), 4092u);
}

template<typename T>
class TreeFixture : public ::testing::Test {
};

typedef ::testing::Types<std::pair<layout_256, tree_sector<uint32_t, uint32_t, 5>>,
                         std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 63>>,
                         std::pair<layout_4096, tree_sector<uint64_t, uint32_t, 287>>>
    Implementations;

static_assert(sizeof(tree_node_t<uint32_t, uint32_t, 6>) <= 4096, "sizeof(Node) <= 256");

static_assert(sizeof(tree_node_t<uint64_t, uint32_t, 288>) <= 4096, "sizeof(Node) <= 4096");

TYPED_TEST_SUITE(TreeFixture, Implementations);

TYPED_TEST(TreeFixture, SingleNodeTree) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.exists(), 0);
        ASSERT_EQ(tree.create(), 0);
        ASSERT_EQ(tree.exists(), 1);

        uint32_t found = 0u;

        ASSERT_EQ(tree.find(1, &found), 0);

        ASSERT_EQ(tree.add(1, 1), 0);
        ASSERT_EQ(tree.find(1, &found), 1);
        ASSERT_EQ(found, 1u);

        ASSERT_EQ(tree.add(2, 2), 0);
        ASSERT_EQ(tree.find(2, &found), 1);
        ASSERT_EQ(found, 2u);

        ASSERT_EQ(tree.add(3, 3), 0);
        ASSERT_EQ(tree.find(3, &found), 1);
        ASSERT_EQ(found, 3u);

        ASSERT_EQ(tree.find(4, &found), 0);
    });
}

TYPED_TEST(TreeFixture, SingleNodeTreeGrowingByOneNode) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 8; ++i) {
            uint32_t found = 0u;
            ASSERT_EQ(tree.add(i, i), 0);
            EXPECT_EQ(tree.find(i, &found), 1);
            ASSERT_EQ(found, i);
        }
    });
}

TYPED_TEST(TreeFixture, SingleNodeTreeGrowingByTwoNodes) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 16; ++i) {
            uint32_t found = 0u;
            ASSERT_EQ(tree.add(i, i), 0);
            EXPECT_EQ(tree.find(i, &found), 1);
            ASSERT_EQ(found, i);
        }

        for (auto i = 1u; i < 16; ++i) {
            uint32_t found = 0u;
            EXPECT_EQ(tree.find(i, &found), 1);
            ASSERT_EQ(found, i);
        }
    });
}

TYPED_TEST(TreeFixture, TreeWith1024Node1Reachable) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        suppress_logs sl;

        for (auto i = 1u; i < 1024; ++i) {
            ASSERT_EQ(tree.add(i, i), 0);

            uint32_t found = 0u;
            EXPECT_EQ(tree.find(1, &found), 1);
            ASSERT_EQ(found, 1u);
        }
    });
}

TYPED_TEST(TreeFixture, TreeAllReachableAsAdded) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 1024; ++i) {
            ASSERT_EQ(tree.add(i, i), 0);

            // Downgraded because of performance.
            if (i % 16 == 0) {
                for (auto j = 1u; j < i; ++j) {
                    uint32_t found = 0u;
                    EXPECT_EQ(tree.find(j, &found), 1);
                    ASSERT_EQ(found, j);
                }
            }
        }
    });
}

TYPED_TEST(TreeFixture, TreeWith1024) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        {
            suppress_logs sl;

            for (auto i = 1u; i < 1024; ++i) {
                uint32_t found = 0u;
                ASSERT_EQ(tree.add(i, i), 0);
                ASSERT_EQ(tree.find(i, &found), 1);
                ASSERT_EQ(found, i);
            }

            for (auto i = 1u; i < 1024; ++i) {
                uint32_t found = 0u;
                EXPECT_EQ(tree.find(i, &found), 1);
                ASSERT_EQ(found, i);
            }
        }

        EXPECT_EQ(tree.log(), 0);
    });
}
