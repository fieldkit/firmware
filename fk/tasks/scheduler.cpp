#include <lwcron/lwcron.h>

#include "clock.h"
#include "tasks/tasks.h"
#include "state_ref.h"
#include "scheduling.h"
#include "hal/random.h"
#include "battery_status.h"
#include "deep_sleep.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

#include "modules/refresh_modules_worker.h"

namespace fk {

FK_DECLARE_LOGGER("schedule");

static CurrentSchedules get_config_schedules();
static bool has_schedule_changed(CurrentSchedules &running);
static bool has_module_topology_changed(Topology &existing);
static void update_allow_deep_sleep(bool enabled);
static bool get_can_launch_captive_readings();

static ScheduledTime get_next_task_time(lwcron::Task &task) {
    auto now = get_clock_now();
    auto next_task_time = task.getNextTime(lwcron::DateTime{ now }, 0);
    auto remaining_seconds = next_task_time - now;
    return {
        .now = now,
        .time = next_task_time,
        .seconds = remaining_seconds,
    };
}

static void update_task_upcoming(ReadingsTask &readings_job) {
    auto scheduled = get_next_task_time(readings_job);
    if (scheduled.time == 0) {
        logerror("no next readings scheduled, huge bug!");
    }
    else {
        logdebug("next readings: %" PRIu32 "s (log)", scheduled.seconds);
    }

    auto gs = get_global_state_rw();
    gs.get()->scheduler.readings.upcoming = scheduled;
}

void task_handler_scheduler(void *params) {
    BatteryChecker battery;

    battery.refresh();

    if (!battery.low_power()) {
        FK_ASSERT(fk_start_task_if_necessary(&display_task));
        FK_ASSERT(fk_start_task_if_necessary(&network_task));

        // NOTE: These share the same stack and so they can never be running together.
#if defined(FK_ENABLE_DEBUG_TASK)
        FK_ASSERT(fk_start_task_if_necessary(&debug_task));
#else
        FK_ASSERT(fk_start_task_if_necessary(&gps_task));
#endif
    }
    else {
        get_board()->disable_gps();
        get_board()->disable_wifi();
        update_allow_deep_sleep(true);
    }

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

        IntervalTimer check_for_tasks_timer;
        IntervalTimer check_for_modules_timer;
        IntervalTimer check_battery_timer;
        IntervalTimer check_module_power_timer;
        IntervalTimer enable_allow_deep_sleep_timer;

        scheduler.begin(get_clock_now());

        while (!has_schedule_changed(schedules) && !fk_task_stop_requested()) {
            // This throttles this loop, so we take a pass when we dequeue or timeout.
            Activity *activity = nullptr;
            if (get_ipc()->dequeue_activity(&activity)) {
                if (!enable_allow_deep_sleep_timer.enabled()) {
                    loginfo("deep sleep disabled");
                    update_allow_deep_sleep(false);
                    enable_allow_deep_sleep_timer.mark();
                }

                activity->consumed();

                if (!fk_task_stop_requested()) {
                    if (fk_start_task_if_necessary(&display_task)) {
                        get_ipc()->launch_worker(create_pool_worker<RefreshModulesWorker>());
                    }
                }
            }

            if (fk_task_stop_requested()) {
                loginfo("stop requested");
                break;
            }

            if (enable_allow_deep_sleep_timer.expired(FiveMinutesMs)) {
                loginfo("deep sleep enabled");
                update_allow_deep_sleep(true);
                enable_allow_deep_sleep_timer.disable();
            }

            if (!battery.low_power_dangerous()) {
                if (check_for_tasks_timer.expired(OneSecondMs)) {
                    auto now = get_clock_now();
                    auto time = lwcron::DateTime{ now };
                    if (!scheduler.check(time, 0)) {
                        if (get_can_launch_captive_readings()) {
                            get_ipc()->launch_worker(WorkerCategory::Readings, create_pool_worker<ReadingsWorker>(false, false, ModulePowerState::AlwaysOn));
                        }
                    }
                    else {
                        update_task_upcoming(readings_job);
                    }
                }
            }

            if (!get_ipc()->has_any_running_worker()) {
                DeepSleep deep_sleep;
                deep_sleep.try_deep_sleep(scheduler);
            }

            if (check_for_modules_timer.expired(OneSecondMs)) {
                // Only do this if we haven't enabled power save mode,
                // which we do after the timer passes.  We're also
                // skipping this if we're setup to always power
                // modules on their own.
                if (!ModulesPowerIndividually && enable_allow_deep_sleep_timer.enabled()) {
                    if (has_module_topology_changed(topology)) {
                        loginfo("topology changed: [%s]", topology.string());
                        get_ipc()->launch_worker(create_pool_worker<ScanModulesWorker>());
                        fk_start_task_if_necessary(&display_task);
                    }
                }
            }

            if (check_battery_timer.expired(ThirtySecondsMs)) {
                auto started = fk_uptime();
                loginfo("refreshing battery");
                battery.refresh();
                loginfo("refreshing battery (%" PRIu32 "ms)", fk_uptime() - started);
            }

            if (false && check_module_power_timer.expired(FiveSecondsMs)) {
                auto gs = get_global_state_ro();
                auto time = gs.get()->runtime.readings;
                auto elapsed = fk_uptime() - time;
                if (elapsed > 20000) {
                    if (get_modmux()->any_modules_on(ModulePower::Unknown)) {
                        loginfo("readings-time: %" PRIu32 " (%" PRIu32 ") (disabling mp::unknown)", elapsed, time);
                        if (!get_modmux()->disable_modules(ModulePower::Unknown)) {
                            logerror("disabling module-power::unknown modules");
                        }
                    }
                    if (get_modmux()->any_modules_on(ModulePower::RareStarts)) {
                        loginfo("readings-time: %" PRIu32 " (%" PRIu32 ") (disabling mp::rare)", elapsed, time);
                        if (!get_modmux()->disable_modules(ModulePower::RareStarts)) {
                            logerror("disabling module-power::unknown modules");
                        }
                    }
                }
            }
        }
    }

    loginfo("scheduler exited");
}

static CurrentSchedules get_config_schedules() {
    auto gs = get_global_state_ro();
    return { gs.get(), get_module_factory() };
}

static void update_allow_deep_sleep(bool enabled) {
    auto gs = get_global_state_rw();
    gs.get()->power.allow_deep_sleep = enabled;
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
