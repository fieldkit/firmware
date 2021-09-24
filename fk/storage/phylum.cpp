#include "storage/phylum.h"
#include "memory.h"

namespace fk {

standard_page_buffer_memory::standard_page_buffer_memory(Pool *pool) : pool_(pool) {
}

standard_page_buffer_memory::~standard_page_buffer_memory() {
}

void *standard_page_buffer_memory::alloc_memory(size_t size) {
    return pool_->malloc(size);
}

void standard_page_buffer_memory::free_memory(void *ptr) {
    // Free with pool.
}

void *standard_page_buffer_memory::alloc_page(size_t size) {
    for (auto iter = pages_; iter != nullptr; iter = iter->np) {
        if (iter->position + size <= iter->size) {
            auto ptr = iter->ptr + iter->position;
            iter->position += size;
            return ptr;
        }
    }

    auto page = (uint8_t *)fk_standard_page_malloc(StandardPageSize, "phylum");
    FK_ASSERT(page != nullptr);

    // Allocate new page.
    auto node = (page_t *)pool_->malloc(sizeof(page_t));
    node->ptr = page;
    node->position = 0;
    node->size = StandardPageSize;
    node->np = pages_;
    pages_ = node;

    auto ptr = node->ptr + node->position;
    node->position += size;

    return ptr;
}

void standard_page_buffer_memory::free_page(void *ptr) {
    while (pages_ != nullptr) {
        fk_standard_page_free(pages_->ptr);
        pages_ = pages_->np;
    }
}

Phylum::Phylum(DataMemory *data_memory, Pool &pool) : memory_(data_memory, &buffers_), pool_(&pool), sector_size_(data_memory->geometry().real_page_size) {
}

bool Phylum::begin(bool force_create) {
    buffers_.clear();

    if (sectors_.begin(force_create) != 0) {
        return false;
    }

    if (allocator_.begin() != 0) {
        return false;
    }

    return true;
}

bool Phylum::format() {
    if (!begin(true)) {
        return false;
    }

    directory_type dir{ pc(), 0 };
    if (dir.format() != 0) {
        return false;
    }

    return true;
}

bool Phylum::mount() {
    if (!begin(false)) {
        return false;
    }

    directory_type dir{ pc(), 0 };
    if (dir.mount() != 0) {
        return false;
    }

    return true;
}

bool Phylum::sync() {
    if (sectors_.sync() != 0) {
        return false;
    }

    return true;
}

uint32_t Phylum::bytes_used() {
    return allocator_.allocated() * sector_size_;
}

}
