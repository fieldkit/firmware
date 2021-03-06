#include "flash.h"

#include <hal_flash.h>
#include <hri/hri_nvmctrl_d51.h>

struct flash_descriptor FLASH_0;

uint32_t page_size_ = 0;
uint32_t total_pages_ = 0;

int32_t bl_flash_initialize() {
    hri_mclk_set_AHBMASK_NVMCTRL_bit(MCLK);
    flash_init(&FLASH_0, NVMCTRL);

    page_size_ = flash_get_page_size(&FLASH_0);
    total_pages_ = flash_get_total_pages(&FLASH_0);

    flash_unlock(&FLASH_0, 0, total_pages_);

    return true;
}

int32_t bl_flash_read(uint32_t address, uint8_t *data, int32_t size) {
    return flash_read(&FLASH_0, address, data, size);
}

int32_t bl_flash_write(uint32_t address, uint8_t const *data, int32_t size) {
    return flash_append(&FLASH_0, address, (uint8_t *)data, size);
}

int32_t bl_flash_erase(uint32_t address, int32_t size) {
    int32_t pages = aligned_on(size, page_size_) / page_size_;
    return flash_erase(&FLASH_0, address, pages);
}