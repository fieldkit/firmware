#include <working_buffers.h>

#include "phylum_tests.h"

using namespace phylum;

class BuffersFixture : public PhylumFixture {};

TEST_F(BuffersFixture, SimpleMalloc) {
    malloc_working_buffers buffers{ 256 };

    {
        auto b1 = buffers.allocate(256);
        phydebugf("%d", sizeof(b1));
    }
}
