#include "bad_blocks.h"
#include "config.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("badblocks");

BadBlocks::BadBlocks(DataMemory *memory) : memory_(memory) {
    if (memory_ == nullptr) {
        return;
    }

    pool_ = create_pool_inside("bad-blocks", DefaultWorkerPoolSize);

    geometry_ = memory->geometry();

    auto size = geometry_.number_of_blocks() / 8;

    loginfo("allocating bad blocks table (%" PRIu32 " bytes)", size);
    table_ = (uint8_t *)pool_->malloc(size);
    bzero(table_, size);
}

BadBlocks::~BadBlocks() {
    if (pool_ != nullptr) {
        delete pool_;
    }
}

void BadBlocks::mark_address_as_bad(uint32_t address) {
    FK_ASSERT(pool_ != nullptr);
    auto block = address / geometry_.block_size;
    mark_block_as_bad(block);
}

void BadBlocks::mark_block_as_bad(uint32_t block) {
    FK_ASSERT(pool_ != nullptr);
    loginfo("[" PRADDRESS "] marking block #%" PRIu32 " bad", block * geometry_.block_size, block);
    table_[block / 8] |= (1 << block % 8);
}

bool BadBlocks::is_address_bad(uint32_t address) {
    FK_ASSERT(pool_ != nullptr);
    return is_block_bad(address / geometry_.block_size);
}

bool BadBlocks::is_block_bad(uint32_t block) {
    FK_ASSERT(pool_ != nullptr);
    return table_[block / 8] & (1 << block % 8);
}

}
