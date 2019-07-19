#include "tasks/tasks.h"

#include "hal/hal.h"

#include "clock.h"
#include "storage.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("worker");

void task_handler_worker(void *params) {
    FK_ASSERT(params != nullptr);

    auto worker = reinterpret_cast<Worker*>(params);

    if (false) {
        StatisticsMemory memory{ MemoryFactory::get_data_memory() };
        Storage storage{ &memory };
        if (!storage.begin()) {
            logerror("error opening storage");
            return;
        }
    }

    WorkerContext wc;

    worker->run(wc);
}

}
