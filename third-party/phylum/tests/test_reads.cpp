#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <file_reader.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template <typename T> class ReadFixture : public PhylumFixture {};

typedef ::testing::Types<
    std::pair<layout_256, directory_chain>,
    std::pair<layout_4096, directory_chain>,
    std::pair<layout_4096, directory_tree>>
    Implementations;

TYPED_TEST_SUITE(ReadFixture, Implementations);

TYPED_TEST(ReadFixture, ReadInlineWrite) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello));
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadInlineWrite_WithAttributes) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    attributes_helper attributes{ this->file_cfg() };

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };
        attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello));
        ASSERT_EQ(attributes.u32(ATTRIBUTE_ONE), 1u);
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadInlineWriteMultipleSameBlock) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };
        for (auto i = 0u; i < 3; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello) * 3);
        ASSERT_EQ(reader.position(), strlen(hello) * 3);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadInlineWriteMultipleSeparateBlocks) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        for (auto i = 0u; i < 3; ++i) {
            ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
            file_appender opened{ memory.pc(), &chain, chain.open() };
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_EQ(opened.close(), 0);
        }
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello) * 3);
        ASSERT_EQ(reader.position(), strlen(hello) * 3);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_TwoBlocks) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0u;

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 2 * memory.sector_size() / strlen(hello); ++i) {
            ASSERT_GT(opened.write(hello), 0);
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            ASSERT_GT(nread, 0);
            bytes_read += nread;
            ASSERT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_TwoBlocks_Binary) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    uint8_t buffer[256];
    for (auto i = 0u; i < sizeof(buffer); ++i) {
        buffer[i] = i + 1;
    }
    auto bytes_wrote = 0u;

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 2 * memory.sector_size() / sizeof(buffer); ++i) {
            ASSERT_GT(opened.write(buffer, sizeof(buffer)), 0);
            bytes_wrote += sizeof(buffer);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            ASSERT_GT(nread, 0);
            bytes_read += nread;
            ASSERT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_TwoBlocks_WithAttributes) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0u;

    attributes_helper attributes{ this->file_cfg() };

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 2 * memory.sector_size() / strlen(hello); ++i) {
            ASSERT_GT(opened.write(hello), 0);
            bytes_wrote += strlen(hello);
        }
        attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            ASSERT_GT(nread, 0);
            bytes_read += nread;
            ASSERT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(attributes.u32(ATTRIBUTE_ONE), 1u);
        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_SeveralBlocks) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0u;

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 100; ++i) {
            ASSERT_EQ(opened.write(hello), (int32_t)strlen(hello));
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto bytes_read = 0u;
        while (bytes_read < bytes_wrote) {
            uint8_t buffer[256];
            auto nread = reader.read(buffer, sizeof(buffer));
            ASSERT_GT(nread, 0);
            bytes_read += nread;
            ASSERT_EQ(reader.position(), bytes_read);
        }

        ASSERT_EQ(reader.position(), bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_NullRead) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0;

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 100; ++i) {
            ASSERT_EQ(opened.write(hello), (int32_t)strlen(hello));
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto nread = reader.read(bytes_wrote);
        ASSERT_EQ(nread, bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadDataChain_NullRead_ReadBeyondEof) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";
    auto bytes_wrote = 0;

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        for (auto i = 0u; i < 100; ++i) {
            ASSERT_EQ(opened.write(hello), (int32_t)strlen(hello));
            bytes_wrote += strlen(hello);
        }
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        auto nread = reader.read(bytes_wrote + 1);
        ASSERT_EQ(nread, bytes_wrote);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(ReadFixture, ReadInlineWrite_NullRead) {
    using dir_type = typename TypeParam::second_type;
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you?";

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.close(), 0);
    });

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &chain, chain.open() };

        ASSERT_EQ(reader.read(1024), (int32_t)strlen(hello));
        ASSERT_EQ(reader.position(), strlen(hello));
        ASSERT_EQ(reader.close(), 0);
    });
}
