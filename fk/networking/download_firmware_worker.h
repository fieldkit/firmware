#pragma once

#include "worker.h"

namespace fk {

class DownloadFirmwareWorker : public Worker {
private:

public:
    DownloadFirmwareWorker();

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() override {
        return "fwdownload";
    }

};

FK_ENABLE_TYPE_NAME(DownloadFirmwareWorker);

}
