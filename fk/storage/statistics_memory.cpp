#include "storage/statistics_memory.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

void MemoryStatistics::add(MemoryStatistics s) {
    nreads += s.nreads;
    nwrites += s.nwrites;
    nerases += s.nerases;
    bytes_read += s.bytes_read;
    bytes_wrote += s.bytes_wrote;
}

void MemoryStatistics::log(const char *prefix) const {
    loginfo("%s%" PRIu32 " reads (%" PRIu32 " bytes), %" PRIu32 " writes, (%" PRIu32 " bytes) %" PRIu32 " erases, %" PRIu32 " copies",
            prefix, nreads, bytes_read, nwrites, bytes_wrote, nerases, ncopies);
}

bool StatisticsMemory::begin() {
    return target_->begin();
}

FlashGeometry StatisticsMemory::geometry() const {
    return target_->geometry();
}

int32_t StatisticsMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    statistics_.add_read(length);
    return target_->read(address, data, length, flags);
}

int32_t StatisticsMemory::write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) {
    statistics_.add_write(length);
    return target_->write(address, data, length, flags);
}

int32_t StatisticsMemory::copy_page(uint32_t source, uint32_t destiny, size_t page_size, uint8_t *buffer, size_t buffer_size) {
    statistics_.ncopies++;
    return target_->copy_page(source, destiny, page_size, buffer, buffer_size);
}

int32_t StatisticsMemory::erase(uint32_t address, size_t length) {
    FK_ASSERT(geometry().is_address_valid(address));
    statistics_.nerases += length / geometry().block_size;
    return target_->erase(address, length);
}

int32_t StatisticsMemory::flush() {
    return target_->flush();
}

MemoryStatistics &StatisticsMemory::statistics() {
    return statistics_;
}

}
