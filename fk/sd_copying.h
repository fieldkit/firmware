#pragma once

#include "common.h"
#include "pool.h"
#include "hal/flash.h"
#include "storage/types.h"

namespace fk {

optional<bool> verify_flash_binary_hash(FlashMemory *flash, uint32_t address, uint32_t binary_size_including_hash,
                                        uint32_t page_size, Hash &expected_hash, Pool &pool);

bool copy_memory_to_flash(FlashMemory *flash, uint8_t const *buffer, size_t size, uint32_t address, uint32_t page_size, Pool &pool);

bool copy_sd_to_flash(const char *path, FlashMemory *flash, uint32_t address, uint32_t page_size, Pool &pool);

} // namespace fk
