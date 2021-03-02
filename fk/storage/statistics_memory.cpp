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

void MemoryStatistics::log() const {
    loginfo("%" PRIu32 " reads (%" PRIu32 " bytes) %" PRIu32 " writes (%" PRIu32 " bytes) %" PRIu32 " erases",
            nreads, bytes_read, nwrites, bytes_wrote, nerases);
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

int32_t StatisticsMemory::erase(uint32_t address, size_t length) {
    FK_ASSERT(geometry().is_address_valid(address));
    statistics_.nerases += length / geometry().block_size;
    return target_->erase(address, length);
}

int32_t StatisticsMemory::erase_block(uint32_t address) {
    FK_ASSERT(geometry().is_address_valid(address));
    statistics_.nerases++;
    return target_->erase_block(address);
}

int32_t StatisticsMemory::flush() {
    return target_->flush();
}

MemoryStatistics &StatisticsMemory::statistics() {
    return statistics_;
}

}
