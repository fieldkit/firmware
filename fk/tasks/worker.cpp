#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("task");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto worker = reinterpret_cast<TaskWorker*>(params);
    auto started = fk_uptime();
    worker->run();
    delete worker;

    auto highwater = os_task_highwater(os_task_self());

    loginfo("done (%" PRIu32 "ms) highwater = (%" PRIu32 ")", fk_uptime() - started, highwater);
}

}
