#include <scoped_allocator>

#include "readings_worker.h"

#include "hal/hal.h"
#include "readings_taker.h"

namespace fk {

FK_DECLARE_LOGGER("rw");

void ReadingsWorker::run(WorkerContext &wc, Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ModuleContext mc{ gs.get(), module_bus };
    Storage storage{ MemoryFactory::get_data_memory() }; // NOTE: Not opened!
    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ scanning, storage, get_modmux(), true };
    auto all_readings = readings_taker.take(mc, pool);
    FK_ASSERT(all_readings);

    auto data_pool = new StaticPool<2048>("readings");
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

    get_ipc()->enqueue_data([=](GlobalState *gs) {
        if (gs->modules != nullptr) {
            delete gs->modules->pool;
        }
        gs->modules = modules;
    });
}

}
