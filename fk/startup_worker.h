#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class GlobalState;

class StartupWorker : public Worker {
private:
    fkb_header_t *fkb_header_{ nullptr };
    bool allow_phylum_{ false };

public:
#if defined(__SAMD51__)
    StartupWorker(bool allow_phylum = true);
#else
    StartupWorker(bool allow_phylum = false);
#endif

public:
    void run(Pool &pool) override;

    TaskDisplayInfo display_info() const override {
        return { .name = "", .progress = 0.0f, .visible = false };
    }

public:
    const char *name() const override {
        return "startup";
    }

private:
    bool save_captured_logs(bool free);
    bool load_or_create_state(Pool &pool);
    bool load_state(Storage &storage, GlobalState *gs, Pool &pool);
    bool create_new_state(Storage &storage, GlobalState *gs, Pool &pool);
    fkb_header_t const *fkb_header();

public:
    void fkb_header(fkb_header_t *fkb_header);

private:
    bool load_from_files(Storage &storage, GlobalState *gs, Pool &pool);
    bool load_previous_location(GlobalState *gs, DataOps *ops, Pool &pool);

private:
    bool check_for_upgrading_startup(Pool &pool);
    bool check_for_provision_startup(Pool &pool);
    bool check_for_self_test_startup(Pool &pool);
    bool check_for_interactive_startup(Pool &pool);
    bool check_for_configure_modules_startup(Pool &pool);
    bool check_for_program_modules_startup(Pool &pool);
    bool check_for_lora(Pool &pool);
};

FK_ENABLE_TYPE_NAME(StartupWorker);

} // namespace fk
