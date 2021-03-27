#include <directory_chain.h>
#include <file_appender.h>
#include <file_reader.h>

#include "suite_base.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class ReadSuite : public PhylumSuite {};

typedef ::testing::Types<layout_256, layout_4096> Implementations;

TYPED_TEST_SUITE(ReadSuite, Implementations);

TYPED_TEST(ReadSuite, ReadInlineWrite) {
    TypeParam layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_reader reader{ chain, chain.open(), std::move(file_buffer) };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello));
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadSuite, ReadInlineWriteMultipleSameBlock) {
    TypeParam layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };
        for (auto i = 0u; i < 3; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_reader reader{ chain, chain.open(), std::move(file_buffer) };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello) * 3);
        ASSERT_EQ(reader.position(), strlen(hello) * 3);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadSuite, ReadInlineWriteMultipleSeparateBlocks) {
    TypeParam layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        for (auto i = 0u; i < 3; ++i) {
            ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
            simple_buffer file_buffer{ memory.sector_size() };
            file_appender opened{ chain, chain.open(), std::move(file_buffer) };
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_EQ(opened.flush(), 0);
        }
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_reader reader{ chain, chain.open(), std::move(file_buffer) };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello) * 3);
        ASSERT_EQ(reader.position(), strlen(hello) * 3);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadSuite, ReadDataChain_TwoBlocks) {
    TypeParam layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0u;

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        for (auto i = 0u; i < 2 * memory.sector_size() / strlen(hello); ++i) {
            ASSERT_GT(opened.write(hello), 0);
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_reader reader{ chain, chain.open(), std::move(file_buffer) };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            EXPECT_GT(nread, 0);
            bytes_read += nread;
            EXPECT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadSuite, ReadDataChain_SeveralBlocks) {
    TypeParam layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0u;

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);
        ASSERT_EQ(chain.flush(), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_appender opened{ chain, chain.open(), std::move(file_buffer) };

        for (auto i = 0u; i < 100; ++i) {
            ASSERT_GT(opened.write(hello), 0);
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted([&](directory_chain &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        simple_buffer file_buffer{ memory.sector_size() };
        file_reader reader{ chain, chain.open(), std::move(file_buffer) };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            EXPECT_GT(nread, 0);
            bytes_read += nread;
            EXPECT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}
