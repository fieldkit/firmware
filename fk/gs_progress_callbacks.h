#include "progress.h"
#include "state_manager.h"

namespace fk {

class GlobalStateProgressCallbacks : public ProgressCallbacks {
private:
    uint32_t updated_;

public:
    virtual ~GlobalStateProgressCallbacks();

public:
    void progress(Operation op, float progress) override;

};

}
