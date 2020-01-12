#include <os.h>

#include "self_check_worker.h"
#include "tasks/tasks.h"
#include "hal/hal.h"

namespace fk {

FK_DECLARE_LOGGER("self-check");

SelfCheckWorker::SelfCheckWorker(SelfCheckCallbacks &callbacks) : callbacks_(&callbacks) {
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
    SelfCheck self_check(&noop_display, get_network(), get_modmux(), get_module_leds());
    self_check.check(SelfCheckSettings::defaults(), *callbacks_);
}

}
