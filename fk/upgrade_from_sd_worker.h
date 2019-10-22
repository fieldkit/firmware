#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

enum class SdCardFirmwareOperation {
    Save,
    Load,
};

class UpgradeFirmwareFromSdWorker {
private:
    SdCardFirmwareOperation op_;

public:
    UpgradeFirmwareFromSdWorker(SdCardFirmwareOperation op);

public:
    void run(Pool &pool);

public:
    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "sdupgrade";
    }

public:
    bool save_firmware(const char *path, uint32_t address, size_t bytes, Pool &pool);

    bool load_firmware(const char *path, uint32_t address, Pool &pool);

    void log_other_firmware();


};

}
