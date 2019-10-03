#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("task");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto worker = reinterpret_cast<Worker*>(params);
    auto started = fk_uptime();
    worker->run();
    delete worker;

    loginfo("done (%" PRIu32 "ms)", fk_uptime() - started);
}

}
