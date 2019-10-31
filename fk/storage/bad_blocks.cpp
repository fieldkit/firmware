#include "bad_blocks.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("badblocks");

BadBlocks::BadBlocks(DataMemory *memory) : memory_(memory), geometry_(memory->geometry()) {
    pool_ = create_pool_inside("bad-blocks", DefaultWorkerPoolSize);

    auto size = geometry_.number_of_blocks() / 8;

    loginfo("allocating bad blocks table (%" PRIu32 " bytes)", size);
    table_ = (uint8_t *)pool_->malloc(size);
}

BadBlocks::~BadBlocks() {
    delete pool_;
}

void BadBlocks::mark_address_as_bad(uint32_t address) {
    auto block = address / geometry_.block_size;
    mark_block_as_bad(block);
}

void BadBlocks::mark_block_as_bad(uint32_t block) {
    loginfo("marking block #%" PRIu32 " bad", block);
    table_[block / 8] |= (1 << block % 8);
}

bool BadBlocks::is_address_bad(uint32_t address) {
    return is_block_bad(address / geometry_.block_size);
}

bool BadBlocks::is_block_bad(uint32_t block) {
    return table_[block / 8] & (1 << block % 8);
}

}
