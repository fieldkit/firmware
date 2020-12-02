#include <samd51_common.h>

#include "graceful_shutdown.h"
#include "hal/hal.h"
#include "readings_taker.h"
#include "readings_worker.h"
#include "state_manager.h"

#include "modules/module_factory.h"
#include "modules/scan_modules_worker.h"

namespace fk {

static size_t failures = 0;

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool scan, bool read_only, bool verify)
    : scan_(scan), read_only_(read_only), verify_(verify) {
}

void ReadingsWorker::run(Pool &pool) {
    if (!prepare(pool)) {
        return;
    }

    take(pool);
}

bool ReadingsWorker::prepare(Pool &pool) {
    if (get_ipc()->has_running_worker(WorkerCategory::Readings)) {
        return false;
    }

    auto throttle_info = read_throttle_and_power_save();
    if (throttle_info.throttle) {
        logwarn("readings throttled");
        return false;
    }

    auto lock = storage_mutex.acquire(UINT32_MAX);
    if (scan_) {
        ScanModulesWorker scan_worker;
        scan_worker.run(pool);
    }

    return true;
}

bool ReadingsWorker::take(Pool &pool) {
    auto taken_readings = take_readings(pool);
    if (!taken_readings) {
        return false;
    }

    auto &all_readings = taken_readings->readings;
    if (all_readings.size() == 0) {
        logwarn("no readings");
        return false;
    }

    auto data_pool = create_pool_inside("readings");
    auto modules = new (data_pool) ModulesState(data_pool);

    modules->nmodules = all_readings.size();
    modules->modules = data_pool->malloc<ModuleState>(all_readings.size());
    modules->readings_time = taken_readings->time;
    modules->readings_number = taken_readings->number;

    auto module_num = 0;
    auto has_readings = false;

    for (auto &m : all_readings) {
        auto sensors = data_pool->malloc<SensorState>(m.sensors->nsensors);

        if (m.meta->flags != FK_MODULES_FLAG_INTERNAL) {
            has_readings = true;
        }

        auto status_message = data_pool->copy(m.status_message);

        modules->readings.emplace(ModuleMetaAndReadings{
            .position = m.position,
            .id = nullptr,
            .meta = m.meta,
            .status_message = status_message,
            .sensors = nullptr,
            .readings = m.readings->clone(*data_pool),
            .configuration = m.configuration,
        });

        modules->modules[module_num] = ModuleState{
            .position = m.position,
            .manufacturer = m.meta->manufacturer,
            .kind = m.meta->kind,
            .version = m.meta->version,
            .name = m.meta->name,
            .display_name_key = m.configuration.display_name_key,
            .id = (fk_uuid_t *)data_pool->copy(m.id, sizeof(fk_uuid_t)),
            .flags = m.meta->flags,
            .status_message = status_message,
            .sensors = sensors,
            .nsensors = m.sensors->nsensors,
        };

        for (auto i = 0u; i < m.sensors->nsensors; ++i) {
            sensors[i].name = m.sensors->sensors[i].name;
            sensors[i].unit_of_measure = m.sensors->sensors[i].unitOfMeasure;
            sensors[i].flags = m.sensors->sensors[i].flags;
            sensors[i].has_live_vaue = true;
            sensors[i].live_value = m.readings->get(i);
        }

        module_num++;
    }

    if (!has_readings) {
        failures++;
    } else {
        failures = 0;
    }

    // NOTE Stability HACK
    if (failures == 60) {
        loginfo("too many empty readings, restarting");
        fk_graceful_shutdown();
        fk_restart();
    }

    logdebug("updating global state");

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        if (!read_only_) {
            gs->readings.time = modules->readings_time;
            gs->readings.number = modules->readings_number;
        }

        if (gs->modules != nullptr) {
            delete gs->modules->pool;
        }

        gs->modules = modules;

        gs->update_physical_modules(taken_readings->constructed_modules);

        logdebug("physical updated gs->modules=0x%p .modules=0x%p nmodules=%zd", modules, modules->modules,
                 modules->nmodules);
    });

    return true;
}

ReadingsWorker::ThrottleAndPowerSave ReadingsWorker::read_throttle_and_power_save() {
    auto gs = get_global_state_rw();
    auto power_save = ModulesPowerIndividually || gs.get()->runtime.power_save;
    if (gs.get()->runtime.readings > 0) {
        auto elapsed = fk_uptime() - gs.get()->runtime.readings;
        if (elapsed < TenSecondsMs) {
            return ThrottleAndPowerSave{ true, false };
        }
    }
    gs.get()->runtime.readings = fk_uptime();
    return ThrottleAndPowerSave{ false, power_save };
}

static GpsState const *get_gps_from_global_state(Pool &pool) {
    return get_global_state_ro().get()->location(pool);
}

tl::expected<TakenReadings, Error> ReadingsWorker::take_readings(Pool &pool) {
    auto mm = get_modmux();
    auto modules_lock = mm->lock();
    auto module_bus = get_board()->i2c_module();

    if (!ModulesPowerIndividually) {
        get_modmux()->check_modules();
    }

    auto gps = get_gps_from_global_state(pool);
    ScanningContext ctx{ mm, gps, module_bus, pool };
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool, read_only_ };
    if (!read_only_ && !storage.begin()) {
        logerror("error opening storage...");
        return tl::unexpected<Error>(Error::IO);
    }

    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ storage, get_modmux(), read_only_, verify_ };
    auto modules = get_module_factory().modules();
    auto taken_readings = readings_taker.take(modules, ctx, pool);
    if (!taken_readings) {
        return tl::unexpected<Error>(taken_readings.error());
    }

    return taken_readings;
}

} // namespace fk
