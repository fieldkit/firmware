#include "tasks/tasks.h"

#include "hal/hal.h"

#include "clock.h"
#include "storage.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("worker");

void task_handler_worker(void *params) {
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

    if (!storage.begin()) {
        logerror("error opening storage");
        return;
    }

    memory.log_statistics();
}

}
