#include <directory_chain.h>
#include <file_appender.h>

#include "suite_base.h"
#include "geometry.h"

using namespace phylum;

class WriteSuite_SectorSize_256 : public PhylumSuite {
protected:
    FlashMemory memory{ 256 };

};

TEST_F(WriteSuite_SectorSize_256, WriteInlineOnce) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(3));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteInlineBuffersMultipleSmall) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) * 3 }));
        EXPECT_TRUE(sg.sector(0).end(3));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteInlineMultipleFlushEach) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(5));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteThreeInlineWritesAndTriggerDataChain) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(5, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(0).end(6));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)((strlen(hello) * 3) + (memory.sector_size() / 2 + 8)) }));
        EXPECT_TRUE(sg.sector(1).end(1));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteAppendsToDataChain) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(5, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(0).end(6));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)((strlen(hello) * 4) + (memory.sector_size() / 2 + 8)) }));
        EXPECT_TRUE(sg.sector(1).end(1));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteAppendsToDataChainGrowingToNewBlock) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello, memory.sector_size()), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(5, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(0).end(6));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)242, InvalidSector, 2 }));
        EXPECT_TRUE(sg.sector(1).end(1));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)228, 1, InvalidSector }));
        EXPECT_TRUE(sg.sector(2).end(1));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteAndIncrementAttribute) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        for (auto i = 0u; i < 3; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
        }

        ASSERT_GE(opened.close(), 0);

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(0).end(6));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteAndIncrementAttributeThreeTimes) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        for (auto i = 0u; i < 3; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(3, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 1));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 2));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(6, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(7, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(0).end(8));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteToDataChainAndIncrementAttributeThreeTimes) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        auto hello = "Hello, world! How are you!";

        for (auto i = 0u; i < 3; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }

        for (auto i = 0u; i < 2; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
            ASSERT_EQ(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 2 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(3, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 1));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 2));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(6, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(7, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(0).end(8));

        EXPECT_TRUE(sg.sector(2).header<directory_chain_header_t>({ 0, InvalidSector }));
        EXPECT_TRUE(sg.sector(2).nth<file_data_t>(1, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(2).nth<file_attribute_t>(2, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 4));
        EXPECT_TRUE(sg.sector(2).nth<file_attribute_t>(3, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 5));
        EXPECT_TRUE(sg.sector(2).end(4));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)242, InvalidSector, 3 }));
        EXPECT_TRUE(sg.sector(1).end(1));

        EXPECT_TRUE(sg.sector(3).header<data_chain_header_t>({ (uint16_t)108, 1, InvalidSector }));
        EXPECT_TRUE(sg.sector(3).end(1));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteImmediatelyToDataChain_SingleBlock) {
    auto hello = "Hello, world! How are you!";

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        for (auto i = 0u; i < 5; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        chain.log();

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, InvalidSector }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(0).end(3));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)130, InvalidSector, InvalidSector }));
        EXPECT_TRUE(sg.sector(1).end(1));

        phydebugf("strlen(hello) = %d", strlen(hello));
    });
}

TEST_F(WriteSuite_SectorSize_256, WriteImmediatelyToDataChain_TwoBlocks) {
    auto hello = "Hello, world! How are you!";

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        chain.log();

        sector_geometry sg{ memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, InvalidSector }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 1, 2 } }));
        EXPECT_TRUE(sg.sector(0).end(3));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)242, InvalidSector, 2 }));
        EXPECT_TRUE(sg.sector(1).end(1));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)18, 1, InvalidSector }));
        EXPECT_TRUE(sg.sector(2).end(1));

        phydebugf("strlen(hello) = %d", strlen(hello));
    });
}
