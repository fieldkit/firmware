#include <directory_chain.h>
#include <file_appender.h>

#include "phylum_tests.h"
#include "geometry.h"

using namespace phylum;

template<typename T>
class WriteFixture : public PhylumFixture {
};

typedef ::testing::Types<
    std::pair<layout_256, directory_chain>
    > Implementations;

TYPED_TEST_SUITE(WriteFixture, Implementations);

TYPED_TEST(WriteFixture, WriteInlineOnce) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInlineBuffersMultipleSmall) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteInlineMultipleFlushEach) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
        ASSERT_GT(opened.write(hello), 0);
        ASSERT_EQ(opened.flush(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteThreeInlineWritesAndTriggerDataChain) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

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

TYPED_TEST(WriteFixture, WriteAppendsToDataChain) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

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

TYPED_TEST(WriteFixture, WriteAppendsToDataChainGrowingToNewBlock) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

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
    });
}

TYPED_TEST(WriteFixture, WriteAndIncrementAttribute) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        for (auto i = 0u; i < 3; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
        }

        ASSERT_GE(opened.close(), 0);
    });
}

TYPED_TEST(WriteFixture, WriteAndIncrementAttributeThreeTimes) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        auto hello = "Hello, world! How are you!";

        for (auto i = 0u; i < 3; ++i) {
            opened.u32(ATTRIBUTE_ONE, opened.u32(ATTRIBUTE_ONE) + 1);
            ASSERT_GT(opened.write(hello), 0);
            ASSERT_GE(opened.flush(), 0);
            ASSERT_GE(opened.close(), 0);
        }
    });
}

TYPED_TEST(WriteFixture, WriteToDataChainAndIncrementAttributeThreeTimes) {
    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

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
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_SingleBlock) {
    auto hello = "Hello, world! How are you!";

    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        for (auto i = 0u; i < 5; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        chain.log();
    });
}

TYPED_TEST(WriteFixture, WriteImmediatelyToDataChain_TwoBlocks) {
    auto hello = "Hello, world! How are you!";

    typename TypeParam::first_type layout;
    FlashMemory memory{ layout.sector_size };
    memory.mounted<directory_chain>([&](auto &chain) {
        ASSERT_EQ(chain.touch("data.txt"), 0);

        ASSERT_EQ(chain.find("data.txt", this->file_cfg()), 1);
        file_appender opened{ chain, &chain, chain.open() };

        for (auto i = 0u; i < 10; ++i) {
            ASSERT_GT(opened.write(hello), 0);
        }
        ASSERT_EQ(opened.flush(), 0);

        chain.log();
    });
}
