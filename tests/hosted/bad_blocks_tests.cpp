#include "tests.h"
#include "patterns.h"

#include "pool.h"
#include "hal/linux/linux.h"
#include "storage/storage.h"
#include "clock.h"
#include "state.h"
#include "storage_suite.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class BadBlocksSuite : public StorageSuite {
};

TEST_F(BadBlocksSuite, AttemptingToAllocateABadBlocks) {
    bank(0).mark_block_bad(g_.block_size * 3);
}
