#include <directory_chain.h>
#include <file_appender.h>
#include <tree_sector.h>

#include "suite_base.h"
#include "geometry.h"

using namespace phylum;

TEST(TreeInfo, NodeSizes) {
    ASSERT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 6, 6>), 81u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 6, 6>), 105u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint64_t, 6, 6>), 111u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 64, 64>), 917u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 128, 128>), 1813u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 288, 288>), 4053u);
    ASSERT_EQ(sizeof(tree_node_t<uint64_t, uint32_t, 291, 291>), 4095u);
    ASSERT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 291, 291>), 2931u);
    ASSERT_EQ(sizeof(tree_node_t<uint32_t, uint32_t, 407, 408>), 4091u);
}

template<typename T>
class TreeSuite : public ::testing::Test {
};

typedef ::testing::Types<std::pair<layout_256, tree_sector<uint32_t, uint32_t, 6, 6>>,
                         std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 64, 64>>,
                         std::pair<layout_4096, tree_sector<uint64_t, uint32_t, 256 + 32, 256 + 32>>>
    Implementations;

static_assert(sizeof(tree_node_t<uint32_t, uint32_t, 6, 6>) <= 4096, "sizeof(Node) <= 256");

static_assert(sizeof(tree_node_t<uint64_t, uint32_t, 256 + 32, 256 + 32>) <= 4096, "sizeof(Node) <= 4096");

TYPED_TEST_SUITE(TreeSuite, Implementations);

TYPED_TEST(TreeSuite, SingleNodeTree) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

        ASSERT_EQ(tree.create(), 0);

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

TYPED_TEST(TreeSuite, SingleNodeTreeGrowingByOneNode) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 8; ++i) {
            uint32_t found = 0u;
            ASSERT_EQ(tree.add(i, i), 0);
            EXPECT_EQ(tree.find(i, &found), 1);
            ASSERT_EQ(found, i);
        }
    });
}

TYPED_TEST(TreeSuite, SingleNodeTreeGrowingByTwoNodes) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

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

TYPED_TEST(TreeSuite, TreeWith1024Node1Reachable) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

        ASSERT_EQ(tree.create(), 0);

        for (auto i = 1u; i < 1024; ++i) {
            ASSERT_EQ(tree.add(i, i), 0);

            uint32_t found = 0u;
            EXPECT_EQ(tree.find(1, &found), 1);
            ASSERT_EQ(found, 1u);
        }
    });
}

TYPED_TEST(TreeSuite, TreeAllReachableAsAdded) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

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

TYPED_TEST(TreeSuite, TreeWith1024) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted([&](directory_chain &chain) {
        auto first = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.sectors(), memory.allocator(), simple_buffer{ memory.sector_size() }, first, "tree" };

        ASSERT_EQ(tree.create(), 0);

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
    });
}
