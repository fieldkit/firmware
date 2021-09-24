#include <lwcron/lwcron.h>

#include "runnable.h"

namespace fk {

class DeepSleep {
public:
    bool once();
    bool try_deep_sleep(lwcron::Scheduler &scheduler, Runnable const &runnable);

};

} // namespace fk
