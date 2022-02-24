#include "poll_sensors_worker.h"
#include "hal/random.h"
#include "state_ref.h"
#include "tasks/tasks.h"
#include "update_readings_listener.h"

namespace fk {

FK_DECLARE_LOGGER("psw");

PollSensorsWorker::PollSensorsWorker(bool scan, bool read_only, bool throttle, uint32_t interval)
    : ReadingsWorker(scan, read_only, throttle), interval_(interval) {
}

void PollSensorsWorker::run(Pool &pool) {
    if (!scan(pool)) {
        logwarn("scan failed, continuing");
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
    auto &udp_traffic = gs.get()->debugging.udp_traffic;
    if (udp_traffic.readings_triggered) {
        udp_traffic.start_time = fk_uptime() + fk_random_i32(0, 10);
        udp_traffic.stop_time = udp_traffic.start_time + udp_traffic.duration;
    } else {
        auto running = os_task_is_running(&network_task);
        if (running) {
            os_signal(&network_task, 9);
        }
    }
}

} // namespace fk
