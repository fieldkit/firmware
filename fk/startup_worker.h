#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class StartupWorker {
public:
    void run(Pool &pool);

public:
    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "startup";
    }

private:
    bool load_or_create_state(Storage &storage, Pool &pool);
    bool load_state(Storage &storage, Pool &pool);
    bool create_new_state(Storage &storage, Pool &pool);

};

}
