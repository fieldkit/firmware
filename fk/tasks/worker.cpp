#include "tasks/tasks.h"

#include "hal/hal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("worker");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto worker = reinterpret_cast<Worker*>(params);
    WorkerContext wc;
    worker->run(wc);
    delete worker;
}

}
