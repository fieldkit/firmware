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
    case DesiredState::Restart:
        if (running) {
            os_signal(&network_task, 9);

            auto started = fk_uptime();
            while (os_task_is_running(&network_task))  {
                fk_delay(500);

                if (fk_uptime() - started > TenSecondsMs) {
                    // NOTE Display error?
                    return;
                }
            }

            os_task_start(&network_task);
        }
        else {
            os_task_start(&network_task);
        }
        break;
    }
}

}
