#include "progress.h"
#include "state_manager.h"

namespace fk {

class GlobalStateProgressCallbacks : public ProgressCallbacks {
private:
    uint32_t updated_;

public:
    void progress(Operation op, float progress) override {
        if (updated_ > 0 && fk_uptime() - updated_ < 500) {
            return;
        }
        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
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
};

}
