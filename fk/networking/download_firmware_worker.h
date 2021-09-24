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
    const char *name() const override {
        return "fwdownload";
    }

};

FK_ENABLE_TYPE_NAME(DownloadFirmwareWorker);

}
