#include "tests.h"
#include "worker.h"
#include "collections.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class PhylumSuite : public ::testing::Test {
protected:

};

TEST_F(PhylumSuite, Basic_Format_NoSync_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        Phylum phylum{ data_memory };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_FALSE(phylum.mount());
    }
}

TEST_F(PhylumSuite, Basic_Format_Mount) {
    auto data_memory = MemoryFactory::get_data_memory();
    ASSERT_TRUE(data_memory->begin());

    {
        Phylum phylum{ data_memory };
        ASSERT_FALSE(phylum.mount());
        ASSERT_TRUE(phylum.format());
        ASSERT_TRUE(phylum.sync());
    }

    {
        Phylum phylum{ data_memory };
        ASSERT_TRUE(phylum.mount());
    }
}
