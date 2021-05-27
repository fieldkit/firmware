#pragma once

#include <loading.h>

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

enum class SdCardFirmwareOperation {
    None,
    Load,
};

struct SdCardFirmware {
    SdCardFirmwareOperation operation;
    const char *bootloader;
    const char *main;
    bool swap;
    bool compare;
    uint32_t delay;
};

class UpgradeFirmwareFromSdWorker : public Worker {
private:
    SdCardFirmware params_;

public:
    explicit UpgradeFirmwareFromSdWorker(SdCardFirmware &params);

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "sdupgrade";
    }

public:
    bool load_firmware(const char *path, uint32_t address, Pool &pool);

    bool log_file_firmware(const char *path, fkb_header_t *header, Pool &pool);

    void log_other_firmware();

    bool has_file(const char *path);

};

FK_ENABLE_TYPE_NAME(UpgradeFirmwareFromSdWorker);

}
