#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

class UpgradeFirmwareFromSdWorker {
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


};

}
