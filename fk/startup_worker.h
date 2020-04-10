#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class GlobalState;

class StartupWorker : public Worker {
public:
    void run(Pool &pool) override;

    TaskDisplayInfo display_info() const override {
        return { .name = "", .progress = 0.0f, .visible = false };
    }

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() const override {
        return "startup";
    }

private:
    bool load_or_create_state(Storage &storage, Pool &pool);
    bool load_state(Storage &storage, GlobalState *gs, Pool &pool);
    bool load_previous_location(Storage &storage, GlobalState *gs, Pool &pool);
    bool create_new_state(Storage &storage, GlobalState *gs, Pool &pool);
    bool save_startup_diagnostics();

private:
    bool check_for_lora(Pool &pool);
    bool check_for_interactive_startup();

};

FK_ENABLE_TYPE_NAME(StartupWorker);

}
