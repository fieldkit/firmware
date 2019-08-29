#include <os.h>

#include "simple_workers.h"

#include "hal/hal.h"
#include "storage/storage.h"
#include "tasks/tasks.h"
#include "state_progress.h"

namespace fk {

FK_DECLARE_LOGGER("fsck");

FsckWorker::FsckWorker() : Worker() {
}

FsckWorker::FsckWorker(Pool *pool) : Worker(pool) {
}

void FsckWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    FK_ASSERT(lock);

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        GlobalStateProgressCallbacks progress;
        storage.fsck(&progress);
    }
}

WifiToggleWorker::WifiToggleWorker() : Worker() {
}

WifiToggleWorker::WifiToggleWorker(Pool *pool) : Worker(pool) {
}

void WifiToggleWorker::run(Pool &pool) {
    if (os_task_is_running(&network_task)) {
        os_signal(&network_task, 9);
    }
    else {
        os_task_start(&network_task);
    }
}

SelfCheckWorker::SelfCheckWorker(SelfCheckCallbacks *callbacks) : Worker(), callbacks_(callbacks) {
}

SelfCheckWorker::SelfCheckWorker(SelfCheckCallbacks *callbacks, Pool *pool) : Worker(pool), callbacks_(callbacks) {
}

void SelfCheckWorker::run(Pool &pool) {
    if (stop_network()) {
        check();
    }
}

bool SelfCheckWorker::stop_network() {
    auto started = fk_uptime();
    auto signaled = false;
    while (os_task_is_running(&network_task)) {
        if (!signaled) {
            os_signal(&network_task, 9);
            signaled = true;
        }
        fk_delay(250);

        if (fk_uptime() - started > FiveSecondsMs) {
            logwarn("networking never stopped");
            return false;
        }
    }

    return true;
}

void SelfCheckWorker::check() {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);
    NullDisplay noop_display;
    SelfCheck self_check(&noop_display, get_network(), get_modmux());
    self_check.check(SelfCheckSettings{ true, true, true }, *callbacks_);
}

}
