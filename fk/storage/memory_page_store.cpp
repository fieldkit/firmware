#include "storage/memory_page_store.h"

namespace fk {

FK_DECLARE_LOGGER("pages");

MemoryPageStore::MemoryPageStore(DataMemory *target) : target_(target) {
}

int32_t MemoryPageStore::load_page(uint32_t address, uint8_t *ptr, size_t size) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    return target_->read(page_address, ptr, size);
}

int32_t MemoryPageStore::save_page(uint32_t address, uint8_t const *ptr, size_t size, uint16_t start, uint16_t end) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    return target_->write(page_address, ptr, size);
}

}
