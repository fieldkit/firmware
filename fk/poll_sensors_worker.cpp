#include "poll_sensors_worker.h"
#include "tasks/tasks.h"

namespace fk {

PollSensorsWorker::PollSensorsWorker() : ReadingsWorker(true, false) {
}

void PollSensorsWorker::run(Pool &pool) {
    auto running = os_task_is_running(&network_task);
    if (running) {
        os_signal(&network_task, 9);
    }

    if (!prepare(pool)) {
        // return;
    }

    while (true) {
        take(pool);

        pool.clear();

        fk_delay(2000);
    }
}

} // namespace fk
