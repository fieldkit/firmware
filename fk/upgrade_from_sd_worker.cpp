#include <samd51_common.h>

#include <loading.h>
#include <phylum/blake2b.h>

#include "modules/dyn/dyn.h"
#include "upgrade_from_sd_worker.h"

#include "hal/flash.h"
#include "storage/types.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "utilities.h"
#include "state_manager.h"
#include "graceful_shutdown.h"
#include "sd_copying.h"
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

    fkb_log_header(fkbh);

    return true;
}

void UpgradeFirmwareFromSdWorker::log_other_firmware() {
    auto ptr = reinterpret_cast<uint8_t*>(OtherBankAddress + BootloaderSize);
    auto fkbh = fkb_try_header(ptr);
    if (fkbh == NULL) {
        return;
    }

    fkb_log_header(fkbh);
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
            fkb_log_header(running_fkbh);

            if (params_.compare) {
                loginfo("comparing firmware");
                if (fkb_same_header(running_fkbh, &file_header)) {
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

bool UpgradeFirmwareFromSdWorker::has_file(const char *path) {
    auto sd = get_sd_card();
    return sd->is_file(path);
}

bool UpgradeFirmwareFromSdWorker::load_firmware(const char *path, uint32_t address, Pool &pool) {
    return copy_sd_to_flash(path, get_flash(), address, CodeMemoryPageSize, pool);
}

}
