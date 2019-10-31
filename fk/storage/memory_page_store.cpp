#include "storage/memory_page_store.h"

namespace fk {

MemoryPageStore::MemoryPageStore(DataMemory *target) : target_(target) {
}

bool MemoryPageStore::load_page(uint32_t address, uint8_t *ptr, size_t size) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    auto rv = target_->read(page_address, ptr, size) == size;
    // fk_dump_memory("RD-PAGE ", ptr, page_size);
    return rv;
}

bool MemoryPageStore::save_page(uint32_t address, uint8_t const *ptr, size_t size) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    // fk_dump_memory("WR-PAGE ", ptr, page_size);
    return target_->write(page_address, ptr, size) == size;
}

}
