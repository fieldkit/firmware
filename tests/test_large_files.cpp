#include <directory_chain.h>
#include <file_appender.h>
#include <tree_sector.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template<typename T>
class LargeFileFixture : public PhylumFixture {};

typedef ::testing::Types<
    std::pair<layout_256, tree_sector<uint32_t, uint32_t, 5>>,
    std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 63>>,
    std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 287>>,
    std::pair<layout_4096, tree_sector<uint32_t, uint32_t, 405>>>
    Implementations;

TYPED_TEST_SUITE(LargeFileFixture, Implementations);

TYPED_TEST(LargeFileFixture, WriteOneMegabyte) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ chain, &chain, chain.open() };

        phydebugf("suppressing 1MB of writes from debug");
        suppress_logs sl;

        auto written = 0u;
        while (written < 1024u * 1024u) {
            auto wrote = opened.write(lorem1k);
            ASSERT_GT(wrote, 0);
            written += wrote;
        }

        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.log(), 0);
    });
}

TYPED_TEST(LargeFileFixture, WriteOneMegabyteIndexed) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto tree_sector = memory.allocator().allocate();
        typename TypeParam::second_type tree{ memory.buffers(), memory.sectors(), memory.allocator(), tree_sector, "tree" };

        ASSERT_EQ(tree.create(), 0);

        auto written = 0u;
        auto cursor = opened.cursor();
        {
            phydebugf("suppressing 1MB of writes from debug");
            suppress_logs sl;

            while (written < 1024u * 1024u) {
                if (opened.length_sectors() % 16 == 0) {
                    if (written == 0 || opened.cursor().sector != cursor.sector) {
                        cursor = opened.cursor();

                        // log_configure_level(LogLevels::DEBUG);
                        phydebugf("cursor sector=%d position=%d position_atsos=%d", cursor.sector, cursor.position, cursor.position_at_start_of_sector);
                        // log_configure_level(LogLevels::NONE);

                        ASSERT_EQ(tree.add(cursor.position_at_start_of_sector, cursor.sector), 0);
                    }
                }

                auto wrote = opened.write(lorem1k);
                ASSERT_GT(wrote, 0);
                written += wrote;
            }
            ASSERT_EQ(opened.flush(), 0);
        }

        uint32_t end = UINT32_MAX;
        uint32_t found_key = 0;
        uint32_t found_value = 0;
        ASSERT_EQ(tree.find_last_less_then(end, &found_value, &found_key), 1);

        ASSERT_EQ(found_key, cursor.position_at_start_of_sector);
        ASSERT_EQ(found_value, cursor.sector);

        tree.log();
    });
}
