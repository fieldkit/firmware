#include <os.h>

#include "wifi_toggle_worker.h"
#include "hal/hal.h"
#include "tasks/tasks.h"

namespace fk {

FK_DECLARE_LOGGER("wifi-toggle");

WifiToggleWorker::WifiToggleWorker(WifiToggleWorker::DesiredState desired) : desired_(desired) {
}

void WifiToggleWorker::run(Pool &pool) {
    auto running = os_task_is_running(&network_task);

    switch (desired_) {
    case DesiredState::Enabled:
        if (!running) {
            os_task_start(&network_task);
        }
        break;
    case DesiredState::Disabled:
        if (running) {
            os_signal(&network_task, 9);
        }
        break;
    case DesiredState::Toggle:
        if (running) {
            os_signal(&network_task, 9);
        }
        else {
            os_task_start(&network_task);
        }
        break;
    }
}

}
