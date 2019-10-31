#include <algorithm>

#include "storage/sequential_memory.h"

namespace fk {

SequentialMemory::SequentialMemory(DataMemory *memory) : memory_(memory) {
}

size_t SequentialMemory::read(uint32_t address, uint8_t *data, size_t length) {
    size_t nbytes = 0;

    auto g = memory_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto reading = std::min<size_t>(remaining, left);
        if (!memory_->read(address, p, reading)) {
            return nbytes;
        }

        address += reading;
        remaining -= reading;
        nbytes += reading;
        p += reading;
    }

    return nbytes;
}

size_t SequentialMemory::write(uint32_t address, uint8_t *data, size_t length) {
    size_t nbytes = 0;

    auto g = memory_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto writing = std::min<size_t>(remaining, left);
        if (!memory_->write(address, p, writing)) {
            return nbytes;
        }

        address += writing;
        remaining -= writing;
        nbytes += writing;
        p += writing;
    }

    return nbytes;
}

size_t SequentialMemory::flush() {
    return memory_->flush();
}

}