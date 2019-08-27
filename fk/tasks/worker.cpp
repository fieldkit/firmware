#include "tasks/tasks.h"
#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("worker");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto started = fk_uptime();
    auto worker = reinterpret_cast<Worker*>(params);
    auto pool = MallocPool{ "worker", 4096 };
    WorkerContext wc;
    worker->run(wc, pool);
    delete worker;

    loginfo("done (pool = %zd/%zd bytes) (%" PRIu32 "ms)", pool.used(), pool.size(), fk_uptime() - started);
}

}
