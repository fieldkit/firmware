#pragma once

#include "common.h"
#include "pool.h"
#include "hal/flash.h"

namespace fk {

bool copy_memory_to_flash(FlashMemory *flash, uint8_t const *buffer, size_t size, uint32_t address, uint32_t page_size, Pool &pool);

bool copy_sd_to_flash(const char *path, FlashMemory *flash, uint32_t address, uint32_t page_size, Pool &pool);

} // namespace fk
