#include "hal/flash.h"
#include "utilities.h"
#include "config.h"

#if defined(__SAMD51__)
#include <hal_flash.h>
#include <hri/hri_nvmctrl_d51.h>
#endif

namespace fk {

FK_DECLARE_LOGGER("flash");

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
    return flash_read(&FLASH_0, address, data, size);
}

int32_t Flash::write(uint32_t address, uint8_t const *data, size_t size) {
    return flash_append(&FLASH_0, address, (uint8_t *)data, size);
}

int32_t Flash::erase(uint32_t address, size_t size) {
    auto pages = aligned_on(size, page_size()) / page_size();
    return flash_erase(&FLASH_0, address, pages);
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
