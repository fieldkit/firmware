#include "progress.h"
#include "state_manager.h"

namespace fk {

class GlobalStateProgressCallbacks : public ProgressCallbacks {
private:
    uint32_t updated_{ 0 };

public:
    GlobalStateProgressCallbacks() { }
    virtual ~GlobalStateProgressCallbacks();

public:
    void progress(Operation op, float progress) override;

};

}
