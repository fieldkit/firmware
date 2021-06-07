#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <tree_sector.h>

#include "phylum_tests.h"

using namespace phylum;

template <typename T> class AttributesFixture : public PhylumFixture {};

typedef ::testing::Types<std::pair<layout_256, tree_sector<uint32_t, attribute_value_t<32>, 6>>,
                         std::pair<layout_4096, tree_sector<uint32_t, attribute_value_t<256>, 15>>>
    Implementations;

TYPED_TEST_SUITE(AttributesFixture, Implementations);

TYPED_TEST(AttributesFixture, Initialize) {
    using layout_type = typename TypeParam::first_type;
    using tree_type = typename TypeParam::second_type;
    using value_type = typename tree_type::value_type;

    phydebugf("sizeof(tree_node<uint32_t, attribute_value_t<256>, 15>) = %zu",
              sizeof(tree_node_t<uint32_t, attribute_value_t<256>, 15>));
    phydebugf("sizeof(tree_node<uint32_t, attribute_value_t<32>, 6>) = %zu",
              sizeof(tree_node_t<uint32_t, attribute_value_t<32>, 6>));

    layout_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &dir) {
        auto first = memory.allocator().allocate();
        tree_type tree{ memory.pc(), tree_ptr_t{ first, first }, "attrs-tree" };

        ASSERT_EQ(tree.create(), 0);

        ASSERT_EQ(tree.add(1, value_type{ 100 }), 0);

        ASSERT_EQ(tree.add(2, value_type{ 100 }), 0);

        value_type value;
        ASSERT_EQ(tree.find(1, &value), 1);

        phydebug_dump_memory("attr ", (uint8_t *)&value, sizeof(value));
    });
}
