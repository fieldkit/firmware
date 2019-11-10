#include "readings_worker.h"
#include "hal/hal.h"
#include "readings_taker.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool read_only) : read_only_(read_only) {
}

void ReadingsWorker::run(Pool &pool) {
    if (should_throttle()) {
        logwarn("readings throttled");
        return;
    }

    auto taken_readings = take_readings(pool);
    if (!taken_readings) {
        return;
    }

    auto &all_readings = taken_readings->readings;

    auto data_pool = create_pool_inside("readings", DefaultWorkerPoolSize);
    auto modules = new (data_pool) ModulesState(data_pool);

    modules->nmodules = all_readings.size();
    modules->modules = data_pool->malloc<ModuleState>(all_readings.size());
    modules->readings_time = taken_readings->time;
    modules->readings_number = taken_readings->number;

    auto module_num = 0;

    for (auto &m : all_readings) {
        auto sensors = data_pool->malloc<SensorState>(m.sensors->nsensors);

        modules->readings.emplace(ModuleMetaAndReadings{
                .position = m.position,
                .id = nullptr,
                .meta = m.meta,
                .sensors = nullptr,
                .readings = m.readings->clone(*data_pool),
        });

        modules->modules[module_num] = ModuleState{
            .position = m.position,
            .manufacturer = m.meta->manufacturer,
            .kind = m.meta->kind,
            .version = m.meta->version,
            .name = m.meta->name,
            .id = (fk_uuid_t *)data_pool->copy(m.id, sizeof(fk_uuid_t)),
            .flags = m.meta->flags,
            .sensors = sensors,
            .nsensors = m.sensors->nsensors,
        };

        for (size_t i = 0; i < m.sensors->nsensors; ++i) {
            sensors[i].name = m.sensors->sensors[i].name;
            sensors[i].unitOfMeasure = m.sensors->sensors[i].unitOfMeasure;
            sensors[i].flags = m.sensors->sensors[i].flags;
            sensors[i].has_live_vaue = true;
            sensors[i].live_value = m.readings->get(i);
        }

        module_num++;
    }

    GlobalStateManager gsm;
    gsm.apply([&](GlobalState *gs) {
        if (!read_only_) {
            gs->storage.meta.size = meta_fh_.size;
            gs->storage.meta.block = meta_fh_.record;
            gs->storage.data.size = data_fh_.size;
            gs->storage.data.block = data_fh_.record;
        }

        if (gs->modules != nullptr) {
            delete gs->modules->pool;
        }
        gs->modules = modules;

        gs->update_physical_modules(taken_readings->constructed_modules);
    });
}

bool ReadingsWorker::should_throttle() {
    if (!read_only_) {
        return false;
    }
    auto gs = get_global_state_rw();
    if (gs.get()->runtime.readings > 0) {
        auto elapsed = fk_uptime() - gs.get()->runtime.readings;

        if (elapsed < TenSecondsMs) {
            return true;
        }
    }

    gs.get()->runtime.readings = fk_uptime();
    return false;
}

tl::expected<TakenReadings, Error> ReadingsWorker::take_readings(Pool &pool) {
    auto gs = get_global_state_ro();
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto eeprom = get_board()->lock_eeprom();

    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();

    ScanningContext ctx{ get_modmux(), gs.get(), module_bus };
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, read_only_ };
    if (!read_only_ && !storage.begin()) {
        logerror("error opening storage...");
        return tl::unexpected<Error>(Error::IO);
    }

    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ scanning, storage, get_modmux(), read_only_ };
    auto taken_readings = readings_taker.take(ctx, pool);
    if (!taken_readings) {
        return tl::unexpected<Error>(taken_readings.error());
    }

    meta_fh_ = storage.file_header(Storage::Meta);
    data_fh_ = storage.file_header(Storage::Data);

    return taken_readings;
}

}
