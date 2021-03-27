#include <dhara_map.h>

#include "suite_base.h"

using namespace phylum;

class DharaSuite : public PhylumSuite {};

TEST_F(DharaSuite, Initialize) {
    memory_flash_memory memory{ 4096 };
    dhara_sector_map sectors{ memory };

    ASSERT_EQ(sectors.begin(true), 0);

    sector_allocator allocator{ sectors };
    directory_chain chain{ sectors, allocator, 0, simple_buffer{ sectors.sector_size() } };

    ASSERT_EQ(chain.format(), 0);
}
