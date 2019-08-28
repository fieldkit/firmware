#include <os.h>

#include "simple_workers.h"

#include "hal/hal.h"
#include "storage/storage.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("fsck");

void FsckWorker::run(WorkerContext &wc, Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        storage.fsck();
    }
}

void WifiToggleWorker::run(WorkerContext &wc, Pool &pool) {
    os_signal(&network_task, 9);
}

void SelfCheckWorker::run(WorkerContext &wc, Pool &pool) {
    NullDisplay noop_display;
    SelfCheck self_check(&noop_display, get_network(), get_modmux());
    self_check.check(SelfCheckSettings{ true, true, true }, *callbacks_);
}

}
