#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <file_reader.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template<typename T>
class TruncateFixture : public PhylumFixture {
};

typedef ::testing::Types<
    std::pair<layout_256, directory_chain>,
    std::pair<layout_4096, directory_tree>
    > Implementations;

TYPED_TEST_SUITE(TruncateFixture, Implementations);

TYPED_TEST(TruncateFixture, Truncate_Inline) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.position(), strlen(hello));
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &dir, dir.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello));
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", this->file_cfg(open_file_flags::Truncate)), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.position(), strlen(hello));
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &dir, dir.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello));
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(TruncateFixture, Truncate_DataChain_Simple) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto written = 0u;

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        written = opened.write(lorem1k, memory.sector_size() * 4);
        ASSERT_GT(written, 0u);
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.allocator().disable();

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_reader reader{ memory.pc(), &dir, dir.open() };

        auto nread = reader.read(MaximumNullReadSize);
        ASSERT_EQ(nread, (int32_t)written);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", this->file_cfg(open_file_flags::Truncate)), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        written = opened.write(lorem1k, memory.sector_size() * 4);
        ASSERT_GT(written, 0u);
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_reader reader{ memory.pc(), &dir, dir.open() };

        auto nread = reader.read(MaximumNullReadSize);
        ASSERT_EQ(nread, (int32_t)written);
    });
}
