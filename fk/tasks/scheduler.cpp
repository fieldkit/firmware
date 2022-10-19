#include <lwcron/lwcron.h>

#include "clock.h"
#include "tasks/tasks.h"
#include "state_ref.h"
#include "scheduling.h"
#include "hal/random.h"
#include "battery_status.h"
#include "deep_sleep.h"
#include "timer.h"
#include "gps_service.h"
#include "state_manager.h"
#include "graceful_shutdown.h"

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

static ScheduledTime get_next_task_time(uint32_t now, lwcron::Task &task) {
    auto next_task_time = task.getNextTime(lwcron::DateTime{ now }, 0);
    auto remaining_seconds = next_task_time - now;
    return {
        .now = now,
        .time = next_task_time,
        .seconds = remaining_seconds,
    };
}

void task_handler_scheduler(void *params) {
    auto display_off = 0;

    BatteryChecker battery;
    battery.refresh();

    GpsService gps_service{ get_gps() };

    if (!battery.low_power()) {
        FK_ASSERT(fk_start_task_if_necessary(&display_task));

#if !defined(FK_DISABLE_NETWORK)
        FK_ASSERT(fk_start_task_if_necessary(&network_task));
#endif

        if (!gps_service.begin()) {
            logerror("gps");
        }
    } else {
        get_board()->disable_gps();
        get_board()->disable_wifi();
        update_allow_deep_sleep(true);
        display_off += fk_uptime() + 10000;
        get_display()->on();
        get_display()->simple(SimpleScreen{ "low battery" });
    }

    uint32_t signal_checked = 0;
    while (!fk_task_stop_requested(&signal_checked)) {
        auto schedules = get_config_schedules();

        ReadingsTask readings_job{ schedules.readings };
        SynchronizeTimeTask synchronize_time_job{ DefaultSynchronizeTimeInterval };
        BackupTask backup_job{ schedules.backup };
        UploadDataTask upload_data_job{ schedules.network, schedules.network_jitter };
        LoraTask lora_job{ schedules.lora };
        GpsTask gps_job{ schedules.gps, gps_service };
        ServiceModulesTask service_modules_job{ schedules.service_interval };

#if !defined(FK_UNDERWATER)
        lwcron::Task *tasks[7]{ &synchronize_time_job, &readings_job, &upload_data_job, &lora_job, &gps_job,
                                &service_modules_job,  &backup_job };
#else
        lwcron::Task *tasks[4]{ &synchronize_time_job, &readings_job, &backup_job, &gps_job };
#endif
        lwcron::Scheduler scheduler{ tasks };
        Topology topology;

        IntervalTimer enable_allow_deep_sleep_timer{ FiveMinutesMs };
        IntervalTimer every_second{ OneSecondMs };
        IntervalTimer every_thirty_seconds{ ThirtySecondsMs };

        scheduler.begin(get_clock_now());

        while (!has_schedule_changed(schedules) && !fk_task_stop_requested(&signal_checked)) {
            // This throttles this loop, so we take a pass when we dequeue or timeout.
            Activity *activity = nullptr;
            if (get_ipc()->dequeue_activity(&activity)) {
                if (!enable_allow_deep_sleep_timer.enabled()) {
                    loginfo("deep sleep disabled");
                    update_allow_deep_sleep(false);
                    enable_allow_deep_sleep_timer.mark();
                }

                activity->consumed();
            }

            if (fk_task_stop_requested(&signal_checked)) {
                loginfo("stop requested");
                break;
            }

            if (activity != nullptr) {
                if (fk_start_task_if_necessary(&display_task)) {
                    get_ipc()->launch_worker(create_pool_worker<RefreshModulesWorker>());
                }
            }

            if (every_second.expired()) {
                GlobalStateManager gsm;

                if (display_off > 0) {
                    get_display()->off();
                    display_off = 0;
                }

                if (enable_allow_deep_sleep_timer.expired()) {
                    loginfo("deep sleep enabled");
                    update_allow_deep_sleep(true);
                    enable_allow_deep_sleep_timer.disable();
                }

                if (every_thirty_seconds.expired()) {
                    auto started = fk_uptime();
                    loginfo("refreshing battery");
                    battery.refresh();
                    loginfo("refreshing battery (%" PRIu32 "ms)", fk_uptime() - started);
                }

                if (!battery.low_power_dangerous()) {
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

                    auto now = get_clock_now();
                    auto time = lwcron::DateTime{ now };
                    if (!scheduler.check(time, 0)) {
                        if (get_can_launch_captive_readings()) {
                            get_ipc()->launch_worker(WorkerCategory::Readings,
                                                     create_pool_worker<ReadingsWorker>(false, false, true, ModulePowerState::AlwaysOn));
                        }
                    }

                    UpcomingUpdate update;
                    update.readings = get_next_task_time(now, readings_job);
                    update.network = get_next_task_time(now, upload_data_job);
                    update.gps = get_next_task_time(now, gps_job);
                    update.lora = get_next_task_time(now, lora_job);
                    update.backup = get_next_task_time(now, backup_job);
                    gsm.apply_update(update);
                } else {
                    // This avoids showing the user ETAs that never move, as
                    // we're no longer servicing the same fields in the above
                    // update.
                    ScheduledTime zero{};
                    UpcomingUpdate update;
                    update.readings = zero;
                    update.network = zero;
                    update.gps = zero;
                    update.lora = zero;
                    update.backup = zero;
                    gsm.apply_update(update);
                }

                if (!get_ipc()->has_any_running_worker()) {
                    DeepSleep deep_sleep;
                    deep_sleep.try_deep_sleep(scheduler, gps_service);
                } else {
                    if (get_ipc()->has_stalled_workers(WorkerCategory::Readings, FiveMinutesMs)) {
                        logwarn("stalled reading worker, restarting");

                        fk_delay(500);

                        fk_graceful_shutdown();
                    }
                }
            }

            gps_service.service();
        }
    }

    loginfo("scheduler exited");
}

static CurrentSchedules get_config_schedules() {
    auto gs = get_global_state_ro();
    return { gs.get() };
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

} // namespace fk
