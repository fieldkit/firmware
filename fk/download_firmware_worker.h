#pragma once

#include "worker.h"

namespace fk {

class DownloadFirmwareWorker {
private:

public:
    DownloadFirmwareWorker();

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "fwdownload";
    }

};

}
