#include <directory_chain.h>
#include <file_appender.h>

#include "suite_base.h"
#include "geometry.h"

using namespace phylum;

class BasicsSuite : public PhylumSuite {
protected:
    FlashMemory memory{ sector_size() };

protected:
    size_t sector_size() const {
        return 256;
    }
};

TEST_F(BasicsSuite, MountFormatMount) {
    memory.begin(true);

    directory_chain chain{ memory.sectors(), memory.allocator(), 0, simple_buffer{ memory.sector_size() } };
    ASSERT_EQ(chain.mount(), -1);
    ASSERT_EQ(chain.format(), 0);
    ASSERT_EQ(chain.flush(), 0);
    ASSERT_EQ(chain.mount(), 0);
}

TEST_F(BasicsSuite, FormatPersists) {
    memory.begin(true);

    memory.sync([&]() {
        directory_chain chain{ memory.sectors(), memory.allocator(), 0, simple_buffer{ memory.sector_size() } };
        ASSERT_EQ(chain.format(), 0);
        ASSERT_EQ(chain.flush(), 0);
    });

    memory.sync([&]() {
        directory_chain chain{ memory.sectors(), memory.allocator(), 0, simple_buffer{ memory.sector_size() } };
        ASSERT_EQ(chain.mount(), 0);
    });
}

TEST_F(BasicsSuite, FindAndTouch) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.flush(), 0);
        ASSERT_EQ(chain.find("test.logs", open_file_config{}), -1);
        ASSERT_EQ(chain.touch("test.logs"), 0);
        ASSERT_EQ(chain.flush(), 0);
        ASSERT_EQ(chain.find("test.logs", open_file_config{}), 1);
    });
}

TEST_F(BasicsSuite, TouchPersists) {
    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.flush(), 0);
        ASSERT_EQ(chain.touch("test.logs"), 0);
        phydebugf("ok done");
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("test.logs", open_file_config{}), -1);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("test.logs"), 0);
        ASSERT_EQ(chain.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("test.logs", open_file_config{}), 1);
    });
}

TEST_F(BasicsSuite, TouchAndFindMultiple) {
    const char *names[] = {
        "a.txt",
        "b.txt",
        "c.txt",
        "d.txt",
        "e.txt",
        "f.txt",
        "g.txt"
    };
    memory.mounted([&](directory_chain &chain) {
        for (auto name : names) {
            ASSERT_EQ(chain.touch(name), 0);
        }
        ASSERT_EQ(chain.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("nope.txt", open_file_config{}), -1);
        for (auto name : names) {
            ASSERT_EQ(chain.find(name, open_file_config{}), 1);
        }
        ASSERT_EQ(chain.find("nope.txt", open_file_config{}), -1);

        sector_geometry sg{ memory.sectors() };
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

        EXPECT_TRUE(sg.sector(2).header<directory_chain_header_t>({ 1, InvalidSector }));
        EXPECT_TRUE(sg.sector(2).nth<file_entry_t>(1, { names[6] }));
        EXPECT_TRUE(sg.sector(2).end(2));
    });
}
