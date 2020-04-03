#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

enum class SdCardFirmwareOperation {
    Save,
    Load,
};

struct SdCardFirmware {
    SdCardFirmwareOperation operation;
    const char *bootloader;
    const char *main;
    bool swap;
};

class UpgradeFirmwareFromSdWorker : public Worker {
private:
    SdCardFirmware params_;

public:
    UpgradeFirmwareFromSdWorker(SdCardFirmware params);

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "sdupgrade";
    }

public:
    bool save_firmware(const char *path, uint32_t address, size_t bytes, Pool &pool);

    bool load_firmware(const char *path, uint32_t address, Pool &pool);

    void log_other_firmware();

    bool has_file(const char *path);

};

FK_ENABLE_TYPE_NAME(UpgradeFirmwareFromSdWorker);

}
