#include "fsck_worker.h"
#include "hal/hal.h"
#include "storage/storage.h"
#include "gs_progress_callbacks.h"

namespace fk {

FK_DECLARE_LOGGER("fsck");

void FsckWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory(), pool };
    if (storage.begin()) {
        GlobalStateProgressCallbacks progress;
        storage.fsck(&progress);
    }
}

}
