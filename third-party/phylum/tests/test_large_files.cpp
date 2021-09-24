#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <tree_sector.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template<typename T>
class LargeFileFixture : public PhylumFixture {};

template<typename TLayout, typename TDirectory, typename TTree>
struct test_types {
    using layout_type = TLayout;
    using directory_type = TDirectory;
    using tree_type = TTree;
};

typedef ::testing::Types<
    test_types<layout_256, directory_chain, tree_sector<uint32_t, uint32_t, 5>>,
    test_types<layout_2048, directory_tree, tree_sector<uint32_t, uint32_t, 201>>,
    test_types<layout_4096, directory_chain, tree_sector<uint32_t, uint32_t, 63>>,
    test_types<layout_4096, directory_chain, tree_sector<uint32_t, uint32_t, 287>>,
    test_types<layout_4096, directory_chain, tree_sector<uint32_t, uint32_t, 405>>,
    test_types<layout_4096, directory_tree, tree_sector<uint32_t, uint32_t, 201>>,
    test_types<layout_4096, directory_tree, tree_sector<uint32_t, uint32_t, 405>>>
    Implementations;

TYPED_TEST_SUITE(LargeFileFixture, Implementations);

TYPED_TEST(LargeFileFixture, WriteOneMegabyte) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<directory_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

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
}

TYPED_TEST(LargeFileFixture, WriteLargeFileIndexed_ContinuousWrites) {
    using layout_type = typename TypeParam::layout_type;
    using directory_type = typename TypeParam::directory_type;
    using tree_type = typename TypeParam::tree_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<directory_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        auto position_index_sector = memory.allocator().allocate();
        tree_type position_index{ memory.pc(), tree_ptr_t{ position_index_sector }, "pos-idx" };

        auto record_index_sector = memory.allocator().allocate();
        tree_type record_index{ memory.pc(), tree_ptr_t{ record_index_sector }, "rec-idx" };

        ASSERT_EQ(position_index.create(), 0);
        ASSERT_EQ(record_index.create(), 0);

        auto written = 0u;
        auto cursor = opened.cursor();
        auto record_number = 0u;
        {
            phydebugf("suppressing 1MB of writes from debug");
            suppress_logs sl;

            while (written < 1024u * 1024u) {
                if (opened.visited_sectors() % 16 == 0) {
                    if (written == 0 || opened.cursor().sector != cursor.sector) {
                        cursor = opened.cursor();

                        phydebugf("cursor sector=%d position=%d position_atsos=%d record_number=%d",
                                  cursor.sector, cursor.position, cursor.position_at_start_of_sector, record_number);

                        ASSERT_EQ(position_index.add(cursor.position_at_start_of_sector, cursor.sector), 0);
                        ASSERT_EQ(record_index.add(record_number, cursor.position), 0);
                    }
                }

                auto wrote = opened.write(lorem1k);
                ASSERT_GT(wrote, 0);
                written += wrote;
                record_number++;
            }
            ASSERT_EQ(opened.flush(), 0);
        }

        uint32_t end = UINT32_MAX;
        uint32_t found_key = 0;
        uint32_t found_value = 0;
        ASSERT_EQ(position_index.find_last_less_then(end, &found_value, &found_key), 1);

        ASSERT_EQ(found_key, cursor.position_at_start_of_sector);
        ASSERT_EQ(found_value, cursor.sector);

        position_index.log();
    });
}
