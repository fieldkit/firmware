#include <loading.h>

#include "hal/flash.h"
#include "upgrade_from_sd_worker.h"
#include "storage/progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "utilities.h"
#include "config.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("sdupgrade");

void UpgradeFirmwareFromSdWorker::run(Pool &pool) {
    auto bl_path = "fkbl.bin";
    auto main_path = "fk-bundled-fkb.bin";

    save_firmware(bl_path, 0x0, 0x8000, pool);
    save_firmware(main_path, 0x8000, fkb_header.firmware.binary_size, pool);

    load_firmware(bl_path, 0x0, pool);
    load_firmware(main_path, 0x80000, pool);
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

    auto file = sd->open(path, true, pool);
    if (file == nullptr || !file) {
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

bool UpgradeFirmwareFromSdWorker::load_firmware(const char *path, uint32_t address, Pool &pool) {
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    loginfo("loading firmware");

    auto file = sd->open(path, false, pool);
    if (file == nullptr || !file) {
        logerror("unable to open '%s'", path);
        return false;
    }

    auto file_size = file->file_size();

    loginfo("opened %zd bytes", file_size);

    file->seek_beginning();

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
    while (true) {
        auto nread = file->read(buffer, CodeMemoryPageSize);
        if (nread <= 0) {
            break;
        }

        FK_ASSERT(nread == CodeMemoryPageSize);

        get_flash()->write(eeprom_address, buffer, CodeMemoryPageSize);

        total_bytes += nread;
        eeprom_address += nread;

        tracker.update(nread);
    }

    tracker.finished();

    loginfo("done reading %" PRIu32, total_bytes);

    return true;
}

}
