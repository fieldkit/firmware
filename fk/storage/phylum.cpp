#include "storage/phylum.h"

namespace fk {

standard_page_working_buffers::standard_page_working_buffers(size_t buffer_size) : working_buffers(buffer_size) {

}

standard_page_working_buffers::~standard_page_working_buffers() {
}

Phylum::Phylum(DataMemory *data_memory) : sector_size_(data_memory->geometry().page_size), memory_(data_memory) {
}

bool Phylum::sync() {
    return false;
}

}
