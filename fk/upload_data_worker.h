#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class UploadDataWorker {
public:
    UploadDataWorker();

public:
    void run(Pool &pool);

public:
    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() {
        return "upldata";
    }

};

}
