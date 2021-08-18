#include "gs_progress_callbacks.h"
#include "platform.h"

namespace fk {

GlobalStateProgressCallbacks::~GlobalStateProgressCallbacks() {
    progress(Operation::None, 0.0f);
}

void GlobalStateProgressCallbacks::progress(Operation op, float progress) {
    if (op != Operation::None && updated_ > 0 && fk_uptime() - updated_ < 500) {
        return;
    }

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        updated_ = fk_uptime();

        switch (op) {
        case Operation::None: {
            gs->progress.operation = nullptr;
            gs->progress.progress = 0.0f;
            updated_ = 0;
            break;
        }
        case Operation::Download: {
            gs->progress.operation = "Copying";
            gs->progress.progress = progress;
            break;
        }
        case Operation::Exporting: {
            gs->progress.operation = "Exporting";
            gs->progress.progress = progress;
            break;
        }
        case Operation::Upload: {
            gs->progress.operation = "Copying";
            gs->progress.progress = progress;
            break;
        }
        case Operation::Fsck: {
            gs->progress.operation = "Fsck";
            gs->progress.progress = progress;
            break;
        }
        case Operation::Wipe: {
            gs->progress.operation = "Wipe";
            gs->progress.progress = progress;
            break;
        }
        }
    });
}

} // namespace fk
