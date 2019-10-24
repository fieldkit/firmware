#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

enum class SdCardFirmwareOperation {
    Save,
    Load,
};

class UpgradeFirmwareFromSdWorker : public Worker {
private:
    SdCardFirmwareOperation op_;

public:
    UpgradeFirmwareFromSdWorker(SdCardFirmwareOperation op);

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() override {
        return "sdupgrade";
    }

public:
    bool save_firmware(const char *path, uint32_t address, size_t bytes, Pool &pool);

    bool load_firmware(const char *path, uint32_t address, Pool &pool);

    void log_other_firmware();

};

}
