#include <samd51_common.h>

#include "readings_worker.h"
#include "readings_taker.h"
#include "state_manager.h"
#include "graceful_shutdown.h"
#include "hal/hal.h"

#include "modules/module_factory.h"
#include "modules/scan_modules_worker.h"

namespace fk {

static size_t failures = 0;

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool scan, bool read_only) : scan_(scan), read_only_(read_only) {
}

void ReadingsWorker::run(Pool &pool) {
    auto throttle_info = read_throttle_and_power_save();
    if (throttle_info.throttle) {
        logwarn("readings throttled");
        return;
    }

    if (scan_) {
        ScanModulesWorker scan_worker;
        scan_worker.run(pool);
    }

    auto taken_readings = take_readings(pool);
    if (!taken_readings) {
        return;
    }

    auto &all_readings = taken_readings->readings;
    if (all_readings.size() == 0) {
        logwarn("no readings");
        return;
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

        modules->readings.emplace(ModuleMetaAndReadings{
            .position = m.position,
            .id = nullptr,
            .meta = m.meta,
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
    }
    else {
        failures = 0;
    }

    // NOTE Stability HACK
    if (failures == 60) {
        loginfo("too many empty readings, restarting");
        fk_graceful_shutdown();
        fk_restart();
    }

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        if (!read_only_) {
            gs->storage.meta.size = meta_fh_.size;
            gs->storage.meta.block = meta_fh_.record;
            gs->storage.data.size = data_fh_.size;
            gs->storage.data.block = data_fh_.record;
            gs->readings.time = modules->readings_time;
            gs->readings.number = modules->readings_number;
        }

        if (gs->modules != nullptr) {
            delete gs->modules->pool;
        }

        gs->modules = modules;

        gs->update_physical_modules(taken_readings->constructed_modules);
    });
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

tl::expected<TakenReadings, Error> ReadingsWorker::take_readings(Pool &pool) {
    auto mm = get_modmux();
    auto modules_lock = mm->lock();
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto gs = get_global_state_ro();
    auto module_bus = get_board()->i2c_module();

    if (!ModulesPowerIndividually) {
        get_modmux()->check_modules();
    }

    ScanningContext ctx{ mm, gs.get(), module_bus, pool };
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool, read_only_ };
    if (!read_only_ && !storage.begin()) {
        logerror("error opening storage...");
        return tl::unexpected<Error>(Error::IO);
    }

    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ storage, get_modmux(), read_only_ };
    auto modules = get_module_factory().modules();
    auto taken_readings = readings_taker.take(modules, ctx, pool);
    if (!taken_readings) {
        return tl::unexpected<Error>(taken_readings.error());
    }

    meta_fh_ = storage.file_header(Storage::Meta);
    data_fh_ = storage.file_header(Storage::Data);

    return taken_readings;
}

}
