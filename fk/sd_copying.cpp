#include <loading.h>
#include <phylum/blake2b.h>

#include "sd_copying.h"

#include "hal/flash.h"
#include "hal/sd_card.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "storage/types.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("sdcopy");

optional<bool> verify_flash_binary_hash(FlashMemory *flash, uint32_t address, uint32_t binary_size_including_hash, uint32_t page_size, Pool &pool) {
    loginfo("[0x%08" PRIx32 "] verifying hash", address);

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    auto flash_address = address;
    auto total_bytes = (uint32_t)0u;
    auto buffer = (uint8_t *)pool.malloc(page_size);
    while (total_bytes < binary_size_including_hash) {
        auto nread = std::min(page_size, binary_size_including_hash - total_bytes);
        if (!flash->read(flash_address, buffer, page_size)) {
            logerror("error reading flash");
            return nullopt;
        }

        total_bytes += nread;
        flash_address += nread;

        if (total_bytes == binary_size_including_hash) {
            b2b.update(buffer, nread - Hash::Length);
        }
        else {
            b2b.update(buffer, nread);
        }
    }

    Hash actual_hash;
    b2b.finalize(&actual_hash.hash, Hash::Length);

    loginfo("[0x%08" PRIx32 "] done reading %" PRIu32 " bytes", address, total_bytes);

    auto hex_str = bytes_to_hex_string_pool(actual_hash.hash, Hash::Length, pool);
    loginfo("[0x%08" PRIx32 "] hash: %s", address, hex_str);

    // Read expected hash from the flash memory device, it's always
    // occupying the end of the binary.
    Hash expected_hash;
    auto hash_address = address + binary_size_including_hash - Hash::Length;
    if (!flash->read(hash_address, (uint8_t *)&expected_hash.hash, Hash::Length)) {
        logerror("error reading hash");
        return nullopt;
    }

    // Compare the hash we calculated with the one that was just
    // written to memory.
    auto success = false;
    if (memcmp(&expected_hash.hash, &actual_hash.hash, Hash::Length) != 0) {
        logerror("[0x%08" PRIx32 "] hash mismatch!", address);
        fk_dump_memory("expected ", (uint8_t *)&expected_hash.hash, Hash::Length);
        fk_dump_memory("actual   ", (uint8_t *)&actual_hash.hash, Hash::Length);
    } else {
        loginfo("[0x%08" PRIx32 "] hash is good!", address);
        success = true;
    }

    return success;
}

bool copy_sd_to_flash(const char *path, FlashMemory *flash, uint32_t address, uint32_t page_size, Pool &pool) {
    // Open the file and get the file size.
    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    if (!sd->is_file(path)) {
        logwarn("no such file %s", path);
        return false;
    }

    loginfo("[0x%08" PRIx32 "] loading binary %s", address, path);
    auto file = sd->open(path, OpenFlags::Read, pool);
    if (file == nullptr || !file->is_open()) {
        logerror("unable to open '%s'", path);
        return false;
    }

    auto file_size = file->file_size();
    if (file_size == 0) {
        logerror("empty file '%s'", path);
        return false;
    }

    loginfo("[0x%08" PRIx32 "] opened, %zd bytes", address, file_size);
    file->seek_beginning();

    // Check to see if a copy is even necessary.
    auto verify_before = verify_flash_binary_hash(flash, address, file_size, page_size, pool);
    if (!verify_before) {
        logerror("error verifying hash");
        return false;
    }

    if (*verify_before) {
        loginfo("[0x%08" PRIx32 "] flash matches %zd bytes", address, file_size);
        return true;
    }

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Download, "sd", "", (uint32_t)file_size };
    auto flash_address = address;

    loginfo("[0x%08" PRIx32 "] erasing to [0x%08" PRIx32 "]", flash_address, (uint32_t)(flash_address + file_size));

    if (!flash->erase(flash_address, file_size)) {
        logerror("error rasing");
    }

    loginfo("[0x%08" PRIx32 "] copying %zd bytes" PRId32, flash_address, file_size);

    // Copy the bytes from the file to memory, using whatever page
    // size we were told to use.
    auto total_bytes = (uint32_t)0u;
    auto buffer = (uint8_t *)pool.malloc(page_size);
    while (total_bytes < file_size) {
        auto nread = file->read(buffer, page_size);
        if (nread <= 0) {
            break;
        }

        if (!flash->write(flash_address, buffer, nread)) {
            logerror("error writing to flash");
            return false;
        }

        total_bytes += nread;
        flash_address += nread;

        tracker.update(nread);
    }

    tracker.finished();

    loginfo("[0x%08" PRIx32 "] done copying %" PRIu32 " bytes", flash_address, total_bytes);

    auto verify_after = verify_flash_binary_hash(flash, address, file_size, page_size, pool);
    if (!verify_after) {
        logerror("error verifying hash");
        return false;
    }

    return *verify_after;
}

} // namespace fk
