#include "storage/phylum.h"
#include "memory.h"

namespace fk {

standard_page_working_buffers::standard_page_working_buffers(size_t buffer_size) : working_buffers(buffer_size) {
    FK_ASSERT(StandardPageSize % buffer_size == 0);
    for (auto n = 0u; n < NumberOfPages; ++n) {
        pages_[n] = nullptr;
    }
}

standard_page_working_buffers::~standard_page_working_buffers() {
    for (auto n = 0u; n < NumberOfPages; ++n) {
        if (pages_[n] != nullptr) {
            fk_standard_page_free(pages_[n]);
            pages_[n] = nullptr;
        }
    }
}

bool standard_page_working_buffers::lend_pages() {
    for (auto n = 0u; n < NumberOfPages; ++n) {
        if (pages_[n] == nullptr) {
            auto page = (uint8_t *)fk_standard_page_malloc(StandardPageSize, "phylum");
            for (auto offset = 0u; offset < StandardPageSize; offset += buffer_size()) {
                lend(page + offset, buffer_size());
            }
            pages_[n] = page;
        }
    }

    return true;
}

Phylum::Phylum(DataMemory *data_memory) : sector_size_(data_memory->geometry().real_page_size), memory_(data_memory) {
}

bool Phylum::begin(bool force_create) {
    if (!buffers_.lend_pages()) {
        return false;
    }

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

}
