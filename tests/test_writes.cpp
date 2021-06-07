#include <directory_chain.h>
#include <directory_tree.h>
#include <file_appender.h>
#include <file_reader.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template<typename T>
class WriteFixture : public PhylumFixture {
};

typedef ::testing::Types<
    std::pair<layout_256, directory_chain>,
    std::pair<layout_4096, directory_tree>
    > Implementations;

TYPED_TEST_SUITE(WriteFixture, Implementations);

TYPED_TEST(WriteFixture, WriteInline_Once) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInline_TwiceSeparately_Appends) {
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
        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        ASSERT_EQ(opened.position(), strlen(hello));
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.position(), strlen(hello) * 2);
        ASSERT_EQ(opened.flush(), 0);
    });

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.find("data.txt", open_file_config{ }), 1);
        file_reader reader{ memory.pc(), &dir, dir.open() };

        ASSERT_EQ(reader.position(), 0u);

        uint8_t buffer[256];
        ASSERT_EQ(reader.read(buffer, sizeof(buffer)), (int32_t)strlen(hello) * 2);
        ASSERT_EQ(reader.position(), strlen(hello) * 2);
        ASSERT_EQ(reader.close(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInline_MultipleSmall_Buffers) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInline_Multiple_FlushEach) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInline_ThreeWritesAndTriggerDataChain) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteAppends_DataChain) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

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
    });
}

TYPED_TEST(WriteFixture, WriteAppends_DataChainGrowingToNewBlock) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

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

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        data_chain dc{ memory.pc(), dir.open().chain };
        ASSERT_EQ(dc.log(true), 0);
    });
}

TYPED_TEST(WriteFixture, WriteAndIncrementAttribute) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        for (auto i = 0u; i < 3; ++i) {
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
        }

        ASSERT_GE(opened.close(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteAndIncrementAttributeThreeTimes) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        for (auto i = 0u; i < 3; ++i) {
            file_appender opened{ memory.pc(), &dir, dir.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }
    });
}

TYPED_TEST(WriteFixture, WriteToDataChainAndIncrementAttributeThreeTimes) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        auto hello = "Hello, world! How are you!";

        attributes_helper attributes{ this->file_cfg() };

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        for (auto i = 0u; i < 3; ++i) {
            phydebugf("opening now");

            file_appender opened{ memory.pc(), &dir, dir.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.close(), 0);
        }

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        for (auto i = 0u; i < 2; ++i) {
            phydebugf("opening again");

            file_appender opened{ memory.pc(), &dir, dir.open() };
            attributes.u32(ATTRIBUTE_ONE, attributes.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(lorem1k, memory.sector_size() / 2 + 8), 0);
            ASSERT_GE(opened.close(), 0);
        }
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_SingleBlock) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        for (auto i = 0u; i < 5; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_TwoBlocks) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_SeveralBlocks) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ memory.pc(), &dir, dir.open() };

        for (auto i = 0u; i < 100; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        data_chain dc{ memory.pc(), dir.open().chain };
        ASSERT_EQ(dc.log(true), 0);
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_TwoBlocksAppending) {
    using layout_type = typename TypeParam::first_type;
    using dir_type = typename TypeParam::second_type;

    layout_type layout;
    FlashMemory memory{ layout.sector_size };

    auto hello = "Hello, world! How are you!";

    memory.mounted<dir_type>([&](auto &dir) {
        ASSERT_EQ(dir.touch("data.txt"), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);

        file_appender opened{ memory.pc(), &dir, dir.open() };
        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.close(), 0);

        ASSERT_EQ(dir.find("data.txt", this->file_cfg()), 1);
        file_appender again{ memory.pc(), &dir, dir.open() };
        ASSERT_EQ(again.seek(), 0);
        ASSERT_EQ(opened.position(), again.position());
        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(again.write(hello), 0);
        }
        ASSERT_EQ(again.close(), 0);

        ASSERT_EQ(again.position(), strlen(hello) * 20);
    });
}
