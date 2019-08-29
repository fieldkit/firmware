#include "progress.h"
#include "state_manager.h"

namespace fk {

class GlobalStateProgressCallbacks : public ProgressCallbacks {
public:
    void progress(Operation op, float progress) override {
        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
                      switch (op) {
                      case Operation::None: {
                          gs->progress.operation = nullptr;
                          gs->progress.progress = 0.0f;
                          break;
                      }
                      case Operation::Download: {
                          gs->progress.operation = "Download..";
                          gs->progress.progress = progress;
                          break;
                      }
                      case Operation::Upload: {
                          gs->progress.operation = "Upload..";
                          gs->progress.progress = progress;
                          break;
                      }
                      case Operation::Fsck: {
                          gs->progress.operation = "Fsck..";
                          gs->progress.progress = progress;
                          break;
                      }
                      }
                  });
    }
};

}
