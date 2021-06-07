#include <super_chain.h>
#include <directory_tree.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class SuperChainFixture : public PhylumFixture {};

template<typename TLayout>
struct test_types {
    using layout_type = TLayout;
};

typedef ::testing::Types<test_types<layout_4096>,
                         test_types<layout_4096>> Implementations;

TYPED_TEST_SUITE(SuperChainFixture, Implementations);

TYPED_TEST(SuperChainFixture, MountFormatMount) {
    using layout_type = typename TypeParam::layout_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.begin(true);

    super_chain super{ memory.pc(), 0 };
    ASSERT_EQ(super.mount(), -1);
    ASSERT_EQ(super.format(), 0);
    ASSERT_EQ(super.mount(), 0);
}

TYPED_TEST(SuperChainFixture, FormatPersists) {
    using layout_type = typename TypeParam::layout_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.begin(true);

    memory.sync([&]() {
        super_chain super{ memory.pc(), 0 };
        ASSERT_EQ(super.format(), 0);
    });

    memory.sync([&]() {
        super_chain super{ memory.pc(), 0 };
        ASSERT_EQ(super.mount(), 0);
    });
}
