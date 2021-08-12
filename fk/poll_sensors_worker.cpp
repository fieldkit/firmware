#include "poll_sensors_worker.h"
#include "state_ref.h"
#include "tasks/tasks.h"
#include "update_readings_listener.h"

namespace fk {

FK_DECLARE_LOGGER("psw");

PollSensorsWorker::PollSensorsWorker(bool scan, bool read_only, uint32_t interval)
    : ReadingsWorker(scan, read_only), interval_(interval) {
}

void PollSensorsWorker::run(Pool &pool) {
    if (!prepare(pool)) {
        logwarn("prepare failed, continuing");
    }

    while (true) {
        if (fk_task_stop_requested(nullptr)) {
            break;
        }

        take_readings();

        fk_delay(interval_);
    }
}

void PollSensorsWorker::take_readings() {
    StandardPool loop_pool{ "poll" };
    UpdateReadingsListener listener{ loop_pool };
    before_readings(loop_pool);
    take(&listener, loop_pool);
    FK_ASSERT(listener.flush() >= 0);
}

void PollSensorsWorker::before_readings(Pool &pool) {
    auto gs = get_global_state_rw();
    loginfo("before-readings");
    if (gs.get()->debugging.udp_traffic.readings_triggered) {
        gs.get()->debugging.udp_traffic.stop_time = fk_uptime() + 1000;
    } else {
        auto running = os_task_is_running(&network_task);
        if (running) {
            os_signal(&network_task, 9);
        }
    }
}

} // namespace fk
