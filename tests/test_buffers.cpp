#include <working_buffers.h>

#include "phylum_tests.h"

using namespace phylum;

class BuffersFixture : public PhylumFixture {};

TEST_F(BuffersFixture, SimpleMalloc) {
    standard_library_malloc buffer_memory;
    working_buffers buffers{ &buffer_memory, 256, 32 };

    {
        auto b1 = buffers.allocate(256);
        phydebugf("%d", sizeof(b1));
    }
}
