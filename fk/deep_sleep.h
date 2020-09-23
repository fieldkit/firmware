#include <lwcron/lwcron.h>

namespace fk {

class DeepSleep {
public:
    bool once();
    bool try_deep_sleep(lwcron::Scheduler &scheduler);

};

} // namespace fk
