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

    get_ipc()->enqueue_data([](GlobalState *gs) {
    });
}

}
