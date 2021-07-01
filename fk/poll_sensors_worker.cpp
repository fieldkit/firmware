#include "poll_sensors_worker.h"
#include "tasks/tasks.h"

namespace fk {

PollSensorsWorker::PollSensorsWorker(bool scan, bool read_only) : ReadingsWorker(scan, read_only) {
}

void PollSensorsWorker::run(Pool &pool) {
    auto running = os_task_is_running(&network_task);
    if (running) {
        os_signal(&network_task, 9);
    }

    if (!prepare(pool)) {
        // return;
    }

    state::NoopReadingsListener listener;
    while (true) {
        {
            StandardPool loop_pool{ "poll" };
            take(&listener, loop_pool);
        }

        fk_delay(2000);
    }
}

} // namespace fk
