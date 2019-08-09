#include "tasks/tasks.h"
#include "config.h"
#include "storage/storage.h"

namespace fk {

FK_DECLARE_LOGGER("misc");

void task_handler_misc(void *params) {

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        storage.fsck();
    }

}

}
