#include <samd51_common.h>

#include <loading.h>
#include <phylum/blake2b.h>

#include "upgrade_from_sd_worker.h"

#include "hal/flash.h"
#include "storage/types.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "utilities.h"
#include "state_manager.h"
#include "graceful_shutdown.h"
#include "config.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("sdupgrade");

#if defined(linux)
fkb_header_t *fkb_try_header(void *ptr) {
    return nullptr;
}
#endif

UpgradeFirmwareFromSdWorker::UpgradeFirmwareFromSdWorker(SdCardFirmware &params) : params_(params) {
}

static void log_fkb_header(fkb_header_t *fkbh) {
    loginfo("[0x%8p] found '%s' / #%" PRIu32 " '%s' flags=0x%" PRIx32 " size=%" PRIu32 " dyntables=+%" PRIu32 " data=%" PRIu32 " bss=%" PRIu32 " got=%" PRIu32 " vtor=0x%" PRIx32, fkbh,
            fkbh->firmware.name, fkbh->firmware.number, fkbh->firmware.version,
            fkbh->firmware.flags, fkbh->firmware.binary_size, fkbh->firmware.tables_offset,
            fkbh->firmware.data_size, fkbh->firmware.bss_size, fkbh->firmware.got_size,
            fkbh->firmware.vtor_offset);

    char hex_hash[(fkbh->firmware.hash_size * 2) + 1];
    bytes_to_hex_string(hex_hash, sizeof(hex_hash), fkbh->firmware.hash, fkbh->firmware.hash_size);

    loginfo("[0x%8p] hash='%s' timestamp=%" PRIu32, fkbh, hex_hash, fkbh->firmware.timestamp);
}

static bool same_header(fkb_header_t *a, fkb_header_t *b) {
    if (a == nullptr || b == nullptr) return false;
    if (a->firmware.hash_size != b->firmware.hash_size) return false;
    return memcmp(a->firmware.hash, b->firmware.hash, b->firmware.hash_size) == 0;
}

bool UpgradeFirmwareFromSdWorker::log_file_firmware(const char *path, fkb_header_t *header, Pool &pool) {
    auto sd = get_sd_card();
    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    if (!sd->is_file(path)) {
        loginfo("no such file %s", path);
        return false;
    }

    auto file = sd->open(path, OpenFlags::Read, pool);
    if (file == nullptr || !file->is_open()) {
        logerror("unable to open '%s'", path);
        return false;
    }

    auto file_size = file->file_size();
    loginfo("opened %zd bytes", file_size);
    if (file_size == 0) {
        logerror("empty file '%s'", path);
        file->close();
        return false;
    }

    file->seek_beginning();

    auto HeaderSize = sizeof(fkb_header_t);
    auto bytes_read = file->read((uint8_t *)header, HeaderSize);
    file->close();

    if (bytes_read != (int32_t)HeaderSize) {
        logerror("error reading header '%s'", path);
        return false;
    }

    auto fkbh = fkb_try_header((void *)header);
    if (fkbh == NULL) {
        logerror("error finding header '%s'", path);
        return false;
    }

    log_fkb_header(fkbh);

    return true;
}

void UpgradeFirmwareFromSdWorker::log_other_firmware() {
    auto ptr = reinterpret_cast<uint8_t*>(OtherBankAddress + BootloaderSize);
    auto fkbh = fkb_try_header(ptr);
    if (fkbh == NULL) {
        return;
    }

    log_fkb_header(fkbh);
}

void UpgradeFirmwareFromSdWorker::run(Pool &pool) {
    auto bl_path = params_.bootloader;
    auto main_path = params_.main;
    // Why not do this?
    // auto lock = sd_mutex.acquire(UINT32_MAX);

    GlobalStateManager gsm;

    switch (params_.operation) {
    case SdCardFirmwareOperation::None: {
        break;
    }
    case SdCardFirmwareOperation::Save: {
        if (bl_path != nullptr) {
            if (!save_firmware(bl_path, 0x0, BootloaderSize, pool)) {
                gsm.notify("error saving bl");
                return;
            }
        }

        if (main_path != nullptr) {
            if (!save_firmware(main_path, BootloaderSize, fkb_header.firmware.binary_size, pool)) {
                gsm.notify("error saving fk");
                return;
            }
        }

        gsm.notify("saved");
        break;
    }
    case SdCardFirmwareOperation::Load: {
        if (main_path != nullptr) {
            fkb_header_t file_header;
            if (!log_file_firmware(main_path, &file_header, pool)) {
                gsm.notify("error inspecting fk");
                return;
            }

            auto ptr = reinterpret_cast<uint8_t *>(PrimaryBankAddress + BootloaderSize);
            auto running_fkbh = fkb_try_header(ptr);
            if (running_fkbh == NULL) {
                gsm.notify("fatal error");
                return;
            }

            loginfo("running firmware");
            log_fkb_header(running_fkbh);

            if (params_.compare) {
                loginfo("comparing firmware");
                if (same_header(running_fkbh, &file_header)) {
                    gsm.notify(pool.sprintf("fw #%d", running_fkbh->firmware.number));
                    loginfo("same firmware");
                    return;
                }
                else {
                    loginfo("new firmware");
                }
            }
        }

        if (bl_path != nullptr && has_file(bl_path)) {
            loginfo("loading bootloader");
            if (!load_firmware(bl_path, OtherBankAddress, pool)) {
                gsm.notify("error loading bl");
                return;
            }
        }
        else {
            loginfo("bootloader skipped");
        }

        if (main_path != nullptr) {
            loginfo("loading firmware");
            if (!load_firmware(main_path, OtherBankAddress + BootloaderSize, pool)) {
                gsm.notify("error loading fk");
                return;
            }
        }
        else {
            loginfo("main skipped");
        }

        log_other_firmware();

        fk_logs_flush();

        if (params_.swap) {
            gsm.notify("success, swap!");

            fk_graceful_shutdown();

            fk_delay(params_.delay);
            fk_nvm_swap_banks();
        }
        else {
            gsm.notify("success!");
        }
        break;
    }
    }
}

bool UpgradeFirmwareFromSdWorker::save_firmware(const char *path, uint32_t address, size_t bytes, Pool &pool) {
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    if (sd->is_file(path)) {
        loginfo("deleting %s", path);
        sd->unlink(path);
    }

    loginfo("saving firmware 0x%" PRIx32 " (%zd bytes)", address, bytes);

    auto file = sd->open(path, OpenFlags::Write, pool);
    if (file == nullptr || !file->is_open()) {
        logerror("unable to open '%s'", path);
        return false;
    }

    auto total_bytes = (uint32_t)0u;
    auto eeprom_address = address;

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Download, "SD", "", (uint32_t)bytes };

    auto buffer = (uint8_t *)pool.malloc(CodeMemoryPageSize);

    while (total_bytes < bytes) {
        get_flash()->read(eeprom_address, buffer, CodeMemoryPageSize);

        auto nwrote = file->write(buffer, CodeMemoryPageSize);
        if (nwrote <= 0) {
            break;
        }

        FK_ASSERT(nwrote == CodeMemoryPageSize);

        total_bytes += nwrote;
        eeprom_address += nwrote;

        tracker.update(nwrote);
    }

    tracker.finished();

    auto file_size = file->file_size();

    file->close();

    loginfo("done saving %" PRIu32 " (%zd)", total_bytes, file_size);

    return true;
}

bool UpgradeFirmwareFromSdWorker::has_file(const char *path) {
    auto sd = get_sd_card();
    return sd->is_file(path);
}

bool UpgradeFirmwareFromSdWorker::load_firmware(const char *path, uint32_t address, Pool &pool) {
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    if (!sd->is_file(path)) {
        loginfo("no such file %s", path);
        return false;
    }

    loginfo("loading firmware");

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

    loginfo("opened %zd bytes", file_size);
    file->seek_beginning();

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    GlobalStateProgressCallbacks gs_progress;
    auto tracker = ProgressTracker{ &gs_progress, Operation::Download, "SD", "", (uint32_t)file_size };

    for (auto eeprom_address = address; eeprom_address < address + file_size; ) {
        loginfo("[0x%06" PRIx32 "] erasing", eeprom_address);
        get_flash()->erase(eeprom_address, CodeMemoryBlockSize / CodeMemoryPageSize);
        eeprom_address += CodeMemoryBlockSize;
    }

    auto total_bytes = (uint32_t)0u;
    auto buffer = (uint8_t *)pool.malloc(CodeMemoryPageSize);
    auto eeprom_address = address;
    while (total_bytes < file_size) {
        auto nread = file->read(buffer, CodeMemoryPageSize);
        if (nread <= 0) {
            break;
        }

        get_flash()->write(eeprom_address, buffer, nread);

        total_bytes += nread;
        eeprom_address += nread;

        tracker.update(nread);

        if (tracker.done()) {
            b2b.update(buffer, nread - Hash::Length);
        }
        else {
            b2b.update(buffer, nread);
        }
    }

    tracker.finished();

    Hash hash;
    b2b.finalize(&hash.hash, Hash::Length);

    // Compare the hash we calculated with the one that was
    // just written to memory.
    auto success = false;
    auto eeprom_hash_ptr = reinterpret_cast<uint8_t*>(eeprom_address - Hash::Length);
    if (memcmp(eeprom_hash_ptr, hash.hash, Hash::Length) != 0) {
        logerror("hash mismatch!");
        fk_dump_memory("EXP ", eeprom_hash_ptr, Hash::Length);
        fk_dump_memory("ACT ", hash.hash, Hash::Length);
    }
    else {
        auto hex_str = bytes_to_hex_string_pool(hash.hash, Hash::Length, pool);
        loginfo("hash is good: %s", hex_str);
        success = true;
    }

    loginfo("done reading %" PRIu32, total_bytes);

    return success;
}

}
