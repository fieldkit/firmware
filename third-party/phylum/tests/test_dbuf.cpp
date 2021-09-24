#include <data_chain.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class DelimitedBufferFixture : public PhylumFixture {};

typedef ::testing::Types<layout_256, layout_4096, layout_4096> Implementations;

TYPED_TEST_SUITE(DelimitedBufferFixture, Implementations);

TYPED_TEST(DelimitedBufferFixture, SeekEnd_SkipsTerminator) {
    using layout_type = TypeParam;
    layout_type layout;

    standard_library_malloc buffer_memory;
    working_buffers buffers{ &buffer_memory, layout.sector_size, 32 };
    delimited_buffer db{ buffers.allocate(layout.sector_size) };

    db.emplace<data_chain_header_t>();

    db.terminate();

    auto position_before = db.position();

    db.rewind();

    db.seek_end();

    auto position_after = db.position();

    ASSERT_EQ(position_before, position_after);
}

struct test_data {
    uint32_t value{ 0x123456 };
};

TYPED_TEST(DelimitedBufferFixture, SeekEnd_IgnoresAfterTerminator) {
    using layout_type = TypeParam;
    layout_type layout;

    standard_library_malloc buffer_memory;
    working_buffers buffers{ &buffer_memory, layout.sector_size, 32 };
    delimited_buffer db{ buffers.allocate(layout.sector_size) };

    db.emplace<data_chain_header_t>();

    db.terminate();

    auto position_before = db.position();

    db.emplace<test_data>();

    db.rewind();

    db.seek_end();

    auto position_after = db.position();

    ASSERT_EQ(position_before, position_after);
}
