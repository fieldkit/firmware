#include "bad_blocks.h"
#include "config.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("badblocks");

BadBlocks::BadBlocks(DataMemory *memory, Pool &pool) : memory_(memory), pool_(&pool) {
    FK_ASSERT(memory_ != nullptr);

    geometry_ = memory->geometry();

    size_ = geometry_.number_of_blocks() / 8;

    loginfo("allocating bad blocks table (%" PRIu32 " bytes)", size_);
    table_ = (uint8_t *)pool_->malloc(size_);
    bzero(table_, size_);
}

BadBlocks::~BadBlocks() {
}

void BadBlocks::mark_address_as_bad(uint32_t address) {
    auto block = address / geometry_.block_size;
    mark_block_as_bad(block);
}

void BadBlocks::mark_block_as_bad(uint32_t block) {
    loginfo("[" PRADDRESS "] marking block #%" PRIu32 " bad", block * geometry_.block_size, block);
    table_[block / 8] |= (1 << block % 8) & 0xff;
}

bool BadBlocks::is_address_bad(uint32_t address) const {
    if (!is_address_valid(address)) {
        return false;
    }
    return is_block_bad(address / geometry_.block_size);
}

bool BadBlocks::is_block_bad(uint32_t block) const {
    FK_ASSERT(block < geometry_.number_of_blocks());
    return (table_[block / 8] & (1 << block % 8)) > 0;
}

}
