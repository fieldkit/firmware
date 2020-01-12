#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class GlobalState;

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
    bool load_state(Storage &storage, GlobalState *gs, Pool &pool);
    bool create_new_state(Storage &storage, GlobalState *gs, Pool &pool);
    bool check_for_lora(Pool &pool);
    bool check_for_interactive_startup();

};

FK_ENABLE_TYPE_NAME(StartupWorker);

}
