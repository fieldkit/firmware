#include <directory_chain.h>
#include <file_appender.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

class LayoutFixture_256 : public PhylumFixture {
protected:
    FlashMemory memory{ 256 };
};

TEST_F(LayoutFixture_256, WriteInlineOnce) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(3));
    });
}

TEST_F(LayoutFixture_256, WriteInlineBuffersMultipleSmall) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) * 3 }));
        EXPECT_TRUE(sg.sector(0).end(3));
    });
}

TEST_F(LayoutFixture_256, WriteInlineMultipleFlushEach) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).end(2));
    });
}

TEST_F(LayoutFixture_256, WriteThreeInlineWritesAndTriggerDataChain) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
        ASSERT_EQ(opened.flush(), 0);

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 2, 2 } }));
        EXPECT_TRUE(sg.sector(1).end(3));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)((strlen(hello) * 3) + (memory.sector_size() / 2 + 8)), InvalidSector, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(2).end(1));
    });
}

TEST_F(LayoutFixture_256, WriteAppendsToDataChain) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

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

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 2, 2 } }));
        EXPECT_TRUE(sg.sector(1).end(3));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)((strlen(hello) * 4) + (memory.sector_size() / 2 + 8)), InvalidSector, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(2).end(1));
    });
}

TEST_F(LayoutFixture_256, WriteAppendsToDataChainGrowingToNewBlock) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

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

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 2, 2 } }));
        EXPECT_TRUE(sg.sector(1).end(3));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)241, InvalidSector, 3 }));
        EXPECT_TRUE(sg.sector(2).end(1));

        EXPECT_TRUE(sg.sector(3).header<data_chain_header_t>({ (uint16_t)229, 2, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(3).end(1));
    });
}

TEST_F(LayoutFixture_256, WriteAndIncrementAttribute) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        for (auto i = 0u; i < 3; ++i) {
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
        }

        ASSERT_GE(opened.close(), 0);

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(3, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_attribute_t>(2, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(1).end(3));
    });
}

TEST_F(LayoutFixture_256, WriteAndIncrementAttributeThreeTimes) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        for (auto i = 0u; i < 3; ++i) {
            file_appender opened{ memory.pc(), &chain, chain.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(3, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 1));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 2));
        EXPECT_TRUE(sg.sector(0).end(6));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_attribute_t>(2, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(1).end(3));
    });
}

TEST_F(LayoutFixture_256, WriteToDataChainAndIncrementAttributeThreeTimes) {
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        for (auto i = 0u; i < 3; ++i) {
            file_appender opened{ memory.pc(), &chain, chain.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.close(), 0);
        }

        for (auto i = 0u; i < 2; ++i) {
            file_appender opened{ memory.pc(), &chain, chain.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
            ASSERT_GE(opened.close(), 0);
        }

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(3, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 1));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(4, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(0).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 2));
        EXPECT_TRUE(sg.sector(0).end(6));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(1, { make_file_id("data.txt"), (file_size_t)strlen(hello) }));
        EXPECT_TRUE(sg.sector(1).nth<file_attribute_t>(2, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 3));
        EXPECT_TRUE(sg.sector(1).nth<file_data_t>(3, { make_file_id("data.txt"), head_tail_t{ 2, 2 } }));
        EXPECT_TRUE(sg.sector(1).nth<file_attribute_t>(4, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 4));
        EXPECT_TRUE(sg.sector(1).nth<file_attribute_t>(5, { make_file_id("data.txt"), ATTRIBUTE_ONE, sizeof(uint32_t) }, 5));
        EXPECT_TRUE(sg.sector(1).end(6));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)241, InvalidSector, 3 }));
        EXPECT_TRUE(sg.sector(2).end(1));

        EXPECT_TRUE(sg.sector(3).header<data_chain_header_t>({ (uint16_t)109, 2, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(3).end(1));
    });
}

TEST_F(LayoutFixture_256, WriteImmediatelyToDataChain_SingleBlock) {
    auto hello = "Hello, world! How are you!";

    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 5; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.close(), 0);

        chain.log();

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, InvalidSector }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 1, 1 } }));
        EXPECT_TRUE(sg.sector(0).end(3));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)130, InvalidSector, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(1).end(1));

        phydebugf("strlen(hello) = %d", strlen(hello));
    });
}

TEST_F(LayoutFixture_256, WriteImmediatelyToDataChain_TwoBlocks) {
    auto hello = "Hello, world! How are you!";

    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        chain.log();

        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, InvalidSector }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { "data.txt" }));
        EXPECT_TRUE(sg.sector(0).nth<file_data_t>(2, { make_file_id("data.txt"), head_tail_t{ 1, 2 } }));
        EXPECT_TRUE(sg.sector(0).end(3));

        EXPECT_TRUE(sg.sector(1).header<data_chain_header_t>({ (uint16_t)241, InvalidSector, 2 }));
        EXPECT_TRUE(sg.sector(1).end(1));

        EXPECT_TRUE(sg.sector(2).header<data_chain_header_t>({ (uint16_t)19, 1, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(2).end(1));
    });
}

TEST_F(LayoutFixture_256, TouchAndFindMultiple) {
    const char *names[] = {
        "a.txt",
        "b.txt",
        "c.txt",
        "d.txt",
        "e.txt",
        "f.txt",
        "g.txt"
    };
    memory.mounted<directory_chain>([&](auto &chain) {
        for (auto name : names) {
            ASSERT_EQ(chain.touch(name), 0);
        }
    });

    memory.mounted<directory_chain>([&](auto &chain) {
        sector_geometry sg{ memory.buffers(), memory.sectors() };
        EXPECT_TRUE(sg.sector(0).header<directory_chain_header_t>({ InvalidSector, 1 }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(1, { names[0] }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(2, { names[1] }));
        EXPECT_TRUE(sg.sector(0).nth<file_entry_t>(3, { names[2] }));
        EXPECT_TRUE(sg.sector(0).end(4));

        EXPECT_TRUE(sg.sector(1).header<directory_chain_header_t>({ 0, 2 }));
        EXPECT_TRUE(sg.sector(1).nth<file_entry_t>(1, { names[3] }));
        EXPECT_TRUE(sg.sector(1).nth<file_entry_t>(2, { names[4] }));
        EXPECT_TRUE(sg.sector(1).nth<file_entry_t>(3, { names[5] }));
        EXPECT_TRUE(sg.sector(1).end(4));

        EXPECT_TRUE(sg.sector(2).header<directory_chain_header_t>({ 1, InvalidSector, sector_flags::Tail }));
        EXPECT_TRUE(sg.sector(2).nth<file_entry_t>(1, { names[6] }));
        EXPECT_TRUE(sg.sector(2).end(2));

        ASSERT_EQ(chain.find("nope.txt", open_file_config{}), 0);
        for (auto name : names) {
            phydebugf("finding %s", name);
            ASSERT_EQ(chain.find(name, open_file_config{}), 1);
        }
        ASSERT_EQ(chain.find("nope.txt", open_file_config{}), 0);
    });
}
