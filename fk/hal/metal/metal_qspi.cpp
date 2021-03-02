#include "hal/hal.h"
#include "hal/metal/metal.h"

#include "modules/dyn/dyn.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("qspi");

MetalQspiMemory::MetalQspiMemory() : transport_{ PIN_QSPI_SCK, QSPI_FLASH_CS, PIN_QSPI_IO0, PIN_QSPI_IO1, PIN_QSPI_IO2, PIN_QSPI_IO3 }, flash_{ &transport_ } {
}

bool MetalQspiMemory::begin() {
    if (status_ != Availability::Unknown) {
        return status_ == Availability::Available;
    }

    status_ = Availability::Unavailable;

    pinMode(QSPI_FLASH_CS, OUTPUT);
    digitalWrite(QSPI_FLASH_CS, LOW);

    if (!flash_.begin()) {
        return false;
    }

    status_ = Availability::Available;

    loginfo("qspi jedec id: 0x%" PRIx32, flash_.getJEDECID());
    loginfo("qspi size: 0x%" PRIx32 " pages: %" PRIu32 " page-size: %" PRIu32,
            flash_.size(), (uint32_t)flash_.numPages(), (uint32_t)flash_.pageSize());

    return true;
}

int32_t MetalQspiMemory::execute(uint32_t *got, uint32_t *entry) {
    FK_ASSERT(status_ == Availability::Available);

    // Teach the peripheral how to do reads so we can execute in place.
    flash_.readBuffer(0, nullptr, 0);

    // Dive into the code.
    fk_dyn_run(got, entry);

    return 0;
}

FlashGeometry MetalQspiMemory::geometry() const {
    if (status_ != Availability::Available) {
        return { 0, 0, 0, 0 };
    }
    return { PageSize, BlockSize, NumberOfBlocks, NumberOfBlocks * BlockSize };
}

int32_t MetalQspiMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    auto nread = flash_.readBuffer(address, data, length);
    if (nread != length) {
        return false;
    }
    return true;
}

int32_t MetalQspiMemory::write(uint32_t address, const uint8_t *data, size_t length, MemoryWriteFlags flags) {
    auto nread = flash_.writeBuffer(address, data, length);
    if (nread != length) {
        return false;
    }
    return true;
}

int32_t MetalQspiMemory::erase_block(uint32_t address) {
    auto g = geometry();
    if (!flash_.eraseBlock(address / g.block_size)) {
        return false;
    }
    return true;
}

int32_t MetalQspiMemory::erase(uint32_t address, size_t length) {
    auto g = geometry();
    return for_each_block_between(address, length, g.block_size, [=](uint32_t block_address) {
        return erase_block(block_address);
    });
}

int32_t MetalQspiMemory::flush() {
    return true;
}

}

#endif
