#include "storage/phylum_flash_memory.h"

namespace fk {

PhylumFlashMemory::PhylumFlashMemory(DataMemory *target, phylum::working_buffers *buffers) : target_(target), buffers_(buffers) {
}

bool PhylumFlashMemory::begin() {
    return target_->begin();
}

size_t PhylumFlashMemory::block_size() {
    return target_->geometry().block_size;
}

size_t PhylumFlashMemory::number_blocks() {
    return target_->geometry().nblocks;
}

size_t PhylumFlashMemory::page_size() {
    return target_->geometry().real_page_size;
}

int32_t PhylumFlashMemory::erase(uint32_t address, uint32_t length) {
    return target_->erase(address, length);
}

int32_t PhylumFlashMemory::write(uint32_t address, uint8_t const *data, size_t size) {
    return target_->write(address, data, size, MemoryWriteFlags::None);
}

int32_t PhylumFlashMemory::read(uint32_t address, uint8_t *data, size_t size) {
    return target_->read(address, data, size, MemoryReadFlags::None);
}

int32_t PhylumFlashMemory::copy_page(uint32_t source, uint32_t destiny, size_t size) {
    auto temporary = buffers_->allocate(page_size());
    return target_->copy_page(source, destiny, size, temporary.ptr(), temporary.size());
}

} // namespace fk
