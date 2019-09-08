#include "readings_worker.h"
#include "hal/hal.h"
#include "readings_taker.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool read_only) : read_only_(read_only) {
}

void ReadingsWorker::run(Pool &pool) {
    auto all_readings = take_readings(pool);
    if (!all_readings) {
        return;
    }

    auto data_pool = new StaticPool<DefaultWorkerPoolSize>("readings");
    auto modules = data_pool->malloc_with<ModulesState>(data_pool);

    modules->nmodules = all_readings->size();
    modules->modules = data_pool->malloc<ModuleState>(all_readings->size());

    auto module_num = 0;

    for (auto m : *all_readings) {
        auto sensors = data_pool->malloc<SensorState>(m.sensors->nsensors);

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
    gsm.apply([=](GlobalState *gs) {
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
    });
}

nonstd::optional<ModuleReadingsCollection> ReadingsWorker::take_readings(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto eeprom = get_board()->lock_eeprom();

    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ModuleContext mc{ gs.get(), module_bus };
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, read_only_ };
    if (!read_only_ && !storage.begin()) {
        logerror("error opening storage, wiping...");
        if (!storage.clear()) {
            logerror("wiping storage failed!");
            return nonstd::nullopt;
        }
    }

    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ scanning, storage, get_modmux(), read_only_ };
    auto all_readings = readings_taker.take(mc, pool);
    if (!all_readings) {
        return nonstd::nullopt;
    }

    meta_fh_ = storage.file_header(Storage::Meta);
    data_fh_ = storage.file_header(Storage::Data);

    return all_readings;
}

}
