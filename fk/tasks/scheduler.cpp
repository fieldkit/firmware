#include <lwcron/lwcron.h>

#include "clock.h"
#include "tasks/tasks.h"
#include "state_ref.h"
#include "scheduling.h"
#include "hal/battery_gauge.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static CurrentSchedules get_config_schedules();

static bool has_schedule_changed(CurrentSchedules &running);

static bool has_module_topology_changed(Topology &existing);

static void check_battery();

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
        SynchronizeTimeTask synchronize_time_job{ DefaultSynchronizeTimeInterval };

        lwcron::Task *tasks[6] { &readings_job, &upload_data_job, &lora_job, &gps_job, &service_modules_job, &synchronize_time_job };
        lwcron::Scheduler scheduler{ tasks };
        Topology topology;

        loginfo("module service interval: %" PRIu32 "s", schedules.service_interval);

        scheduler.begin( get_clock_now() );

        auto check_for_tasks_time = fk_uptime() + OneSecondMs;
        auto check_for_modules_time = fk_uptime() + OneSecondMs;
        auto check_battery_time = fk_uptime() + ThirtySecondsMs;

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
                    fk_start_task_if_necessary(&display_task);
                }
                check_for_modules_time = fk_uptime() + OneSecondMs;
            }

            if (check_battery_time < fk_uptime()) {
                check_battery();
                check_battery_time = fk_uptime() + ThirtySecondsMs;
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

    if (existing == topology.value()) {
        return false;
    }

    existing = topology.value();

    {
        auto modules_lock = modules_mutex.acquire(UINT32_MAX);
        get_modmux()->check_modules();
    }

    return true;
}

static void check_battery() {
    if (!get_battery_gauge()->available()) {
        return;
    }

    auto lock = get_modmux()->lock();

    auto battery = get_battery_gauge()->get();
    if (!battery.available) {
        logerror("battery status unavilable");
        return;
    }

    auto gs = get_global_state_rw();
    gs.get()->power.vbus = battery.bus_voltage;
    gs.get()->power.vs = battery.shunted_voltage;
    gs.get()->power.ma = battery.ma;
    gs.get()->power.mw = battery.mw;

    loginfo("battery: v_bus = %fV v_s = %fmV %fmA %fmW", battery.bus_voltage, battery.shunted_voltage, battery.ma, battery.mw);
}

}
