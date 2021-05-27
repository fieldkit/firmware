#include <dhara_map.h>

#include "phylum_tests.h"

using namespace phylum;

class DharaFixture : public PhylumFixture {};

TEST_F(DharaFixture, Initialize) {
    memory_flash_memory memory{ 4096 };
    malloc_working_buffers buffers{ 4096 };
    noop_page_cache page_cache;
    dhara_sector_map sectors{ buffers, memory, &page_cache };

    ASSERT_EQ(sectors.begin(true), 0);

    sector_allocator allocator{ sectors };
    directory_chain chain{ phyctx{ buffers, sectors, allocator }, 0 };

    ASSERT_EQ(chain.format(), 0);
}
