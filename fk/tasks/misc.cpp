#include "tasks/tasks.h"
#include "config.h"
#include "storage/storage.h"
#include "hal/metal/metal.h"

namespace fk {

FK_DECLARE_LOGGER("misc");

void task_handler_misc(void *params) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        storage.fsck();
    }
}

}
