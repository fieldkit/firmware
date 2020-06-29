#include <lwcron/lwcron.h>

#include "clock.h"
#include "tasks/tasks.h"
#include "state_ref.h"
#include "scheduling.h"
#include "hal/random.h"
#include "battery_status.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static CurrentSchedules get_config_schedules();
static bool has_schedule_changed(CurrentSchedules &running);
static bool has_module_topology_changed(Topology &existing);
static void update_power_save(bool enabled);
static bool get_can_launch_captive_readings();

void task_handler_scheduler(void *params) {
    FK_ASSERT(fk_start_task_if_necessary(&display_task));
    FK_ASSERT(fk_start_task_if_necessary(&network_task));
    FK_ASSERT(fk_start_task_if_necessary(&gps_task));

    while (!fk_task_stop_requested()) {
        auto schedules = get_config_schedules();

        ReadingsTask readings_job{ schedules.readings };
        UploadDataTask upload_data_job{ schedules.network, schedules.network_jitter };
        LoraTask lora_job{ schedules.lora };
        GpsTask gps_job{ schedules.gps };
        ServiceModulesTask service_modules_job{ schedules.service_interval };
        SynchronizeTimeTask synchronize_time_job{ DefaultSynchronizeTimeInterval };

        lwcron::Task *tasks[6]{ &synchronize_time_job, &readings_job, &upload_data_job, &lora_job, &gps_job,
                                &service_modules_job };
        lwcron::Scheduler scheduler{ tasks };
        Topology topology;

        loginfo("module service interval: %" PRIu32 "s", schedules.service_interval);

        scheduler.begin( get_clock_now() );

        IntervalTimer check_for_tasks_timer;
        IntervalTimer check_for_modules_timer;
        IntervalTimer check_battery_timer;
        IntervalTimer enable_power_save_timer;

        while (!has_schedule_changed(schedules) && !fk_task_stop_requested()) {
            // This throttles this loop, so we take a pass when we dequeue or timeout.
            Activity *activity = nullptr;
            if (get_ipc()->dequeue_activity(&activity)) {
                if (!enable_power_save_timer.enabled()) {
                    loginfo("power saved disabled");
                    update_power_save(false);
                    enable_power_save_timer.mark();
                }

                activity->consumed();

                if (!fk_task_stop_requested()) {
                    fk_start_task_if_necessary(&display_task);
                }
            }

            if (fk_task_stop_requested()) {
                loginfo("stop requested");
                break;
            }

            if (enable_power_save_timer.expired(FiveMinutesMs)) {
                loginfo("power saved enabled");
                update_power_save(true);
                enable_power_save_timer.disable();
            }

            if (check_for_tasks_timer.expired(OneSecondMs)) {
                auto time = lwcron::DateTime{ get_clock_now() };
                auto seed = fk_random_i32(0, INT32_MAX);
                if (!scheduler.check(time, seed)) {
                    if (get_can_launch_captive_readings()) {
                        get_ipc()->launch_worker(WorkerCategory::Readings, create_pool_worker<ReadingsWorker>(false, false));
                    }
                }
            }

            if (check_for_modules_timer.expired(OneSecondMs)) {
                // Only do this if we haven't enabled power save mode,
                // which we do after enable_power_save_time passes.
                // We're also skipping this if we're setup to always
                // power modules on their own.
                if (!ModulesPowerIndividually && enable_power_save_timer.enabled()) {
                    if (has_module_topology_changed(topology)) {
                        loginfo("topology changed: [%s]", topology.string());
                        get_ipc()->launch_worker(create_pool_worker<ScanModulesWorker>());
                        fk_start_task_if_necessary(&display_task);
                    }
                }
            }

            if (check_battery_timer.expired(ThirtySecondsMs)) {
                BatteryStatus battery;
                battery.refresh();
            }
        }
    }
}

static CurrentSchedules get_config_schedules() {
    auto gs = get_global_state_ro();
    return { gs.get(), get_module_factory() };
}

static void update_power_save(bool enabled) {
    auto gs = get_global_state_rw();
    gs.get()->runtime.power_save = enabled;
}

static bool has_schedule_changed(CurrentSchedules &running) {
    auto config = get_config_schedules();
    return !config.equals(running);
}

static void check_modules() {
    auto modules_lock = modules_mutex.acquire(UINT32_MAX);
    get_modmux()->check_modules();
}

static bool get_can_launch_captive_readings() {
    // We definitely don't if there's nothing connected.
    if (!get_network()->enabled()) {
        return false;
    }

    auto gs = get_global_state_ro();

    // Don't if it's been more than a minute since we had network activity.
    auto since_activity = fk_uptime() - gs.get()->network.state.activity;
    if (since_activity > OneMinuteMs) {
        return false;
    }

    // Will we be throttled?
    auto elapsed = fk_uptime() - gs.get()->runtime.readings;
    if (elapsed < TenSecondsMs) {
        return false;
    }

    // Don't bother if the worker is already running.
    if (get_ipc()->has_running_worker(WorkerCategory::Readings)) {
        return false;
    }

    return true;
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

    check_modules();

    return true;
}

}
