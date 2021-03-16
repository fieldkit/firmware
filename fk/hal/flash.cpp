#include "hal/flash.h"
#include "utilities.h"
#include "config.h"

#if defined(__SAMD51__)
#include <hal_flash.h>
#include <hri/hri_nvmctrl_d51.h>
#endif

namespace fk {

FK_DECLARE_LOGGER("flash");

DataMemoryFlash::DataMemoryFlash(DataMemory *data) : data_(data) {
}

DataMemoryFlash::~DataMemoryFlash() {
    if (data_ != nullptr) {
        if (!data_->begin()) {
            logerror("error returning qspi to read");
        }
    }
}

FlashWriter::FlashWriter(FlashMemory *memory, uint32_t address) : memory_(memory), address_(address), erased_(address) {
}

FlashWriter::~FlashWriter() {
}

int32_t FlashWriter::write(uint8_t const *buffer, size_t size) {
    if (address_ + size > erased_) {
        auto erase_multiple = 64u * 1024u;
        loginfo("[0x%08" PRIx32 "] erasing %zd", erased_, erase_multiple);
        if (!memory_->erase(erased_, erase_multiple)) {
            return 0;
        }

        erased_ += erase_multiple;
    }

    if (!memory_->write(address_, buffer, size)) {
        return 0;
    }

    address_ += size;

    return size;
}

#if defined(__SAMD51__)

struct flash_descriptor FLASH_0;

bool Flash::initialize() {
    hri_mclk_set_AHBMASK_NVMCTRL_bit(MCLK);
    flash_init(&FLASH_0, NVMCTRL);

    page_size_ = flash_get_page_size(&FLASH_0);
    total_pages_ = flash_get_total_pages(&FLASH_0);

    loginfo("initialized: page-size = %" PRIu32 " bytes total-pages = %" PRIu32 "", page_size_, total_pages_);

    FK_ASSERT(page_size_ == CodeMemoryPageSize);

    flash_unlock(&FLASH_0, 0, total_pages_);

    return true;
}

int32_t Flash::read(uint32_t address, uint8_t *data, size_t size) {
    if (flash_read(&FLASH_0, address, data, size) == 0) {
        return size;
    }
    return -1;
}

int32_t Flash::write(uint32_t address, uint8_t const *data, size_t size) {
    if (flash_append(&FLASH_0, address, (uint8_t *)data, size) == 0) {
        return size;
    }
    return -1;
}

int32_t Flash::erase(uint32_t address, size_t size) {
    auto erasing = 0u;
    while (erasing < size) {
        if (flash_erase(&FLASH_0, address + erasing, CodeMemoryBlockSize / CodeMemoryPageSize) < 0) {
            return -1;
        }
        erasing += CodeMemoryBlockSize;
    }
    return size;
}

#else

bool Flash::initialize() {
    return true;
}

int32_t Flash::read(uint32_t address, uint8_t *data, size_t size) {
    return 0;
}

int32_t Flash::write(uint32_t address, uint8_t const *data, size_t size) {
    return 0;
}

int32_t Flash::erase(uint32_t address, size_t pages) {
    return 0;
}

#endif

static Flash flash;

Flash *get_flash() {
    return &flash;
}

}
