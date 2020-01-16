#include <lwcron/lwcron.h>

#include "clock.h"
#include "tasks/tasks.h"
#include "state_ref.h"
#include "scheduling.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static CurrentSchedules get_config_schedules();

static bool has_schedule_changed(CurrentSchedules &running);

static bool has_module_topology_changed(Topology &existing);

void task_handler_scheduler(void *params) {
    FK_ASSERT(fk_start_task_if_necessary(&display_task));
    FK_ASSERT(fk_start_task_if_necessary(&network_task));
    FK_ASSERT(fk_start_task_if_necessary(&gps_task));

    while (true) {
        auto schedules = get_config_schedules();

        ReadingsTask readings_job{ schedules.readings };
        UploadDataTask upload_data_job{ schedules.network };
        LoraTask lora_job{ schedules.lora };
        GpsTask gps_job{ schedules.gps };
        ServiceModulesTask service_modules_job{ schedules.service_interval };

        lwcron::Task *tasks[5] { &readings_job, &upload_data_job, &lora_job, &gps_job, &service_modules_job };
        lwcron::Scheduler scheduler{ tasks };
        Topology topology;

        loginfo("module service interval: %" PRIu32 "s", schedules.service_interval);

        scheduler.begin( get_clock_now() );

        auto check_for_tasks_time = fk_uptime() + OneSecondMs;
        auto check_for_modules_time = fk_uptime() + OneSecondMs;

        while (!has_schedule_changed(schedules)) {
            // This throttles this loop, so we take a pass when we dequeue or timeout.
            Activity *activity = nullptr;
            if (get_ipc()->dequeue_activity(&activity)) {
                activity->consumed();
                fk_start_task_if_necessary(&display_task);
            }

            if (check_for_tasks_time < fk_uptime()) {
                auto time = lwcron::DateTime{ get_clock_now() };
                scheduler.check(time);
                check_for_tasks_time = fk_uptime() + OneSecondMs;
            }

            if (check_for_modules_time < fk_uptime()) {
                if (has_module_topology_changed(topology)) {
                    loginfo("topology changed: [%s]", topology.string());
                    get_ipc()->launch_worker(create_pool_worker<ScanModulesWorker>());
                }
                check_for_modules_time = fk_uptime() + OneSecondMs;
            }
        }

        loginfo("configuration changed");
    }
}

static CurrentSchedules get_config_schedules() {
    auto gs = get_global_state_ro();
    return { gs.get(), get_module_factory() };
}

static bool has_schedule_changed(CurrentSchedules &running) {
    auto config = get_config_schedules();
    return !config.equals(running);
}

static bool has_module_topology_changed(Topology &existing) {
    auto topology = get_modmux()->get_topology();
    if (!topology) {
        return false;
    }

    if (existing == topology) {
        return false;
    }

    existing = topology.value();

    return true;
}

}
