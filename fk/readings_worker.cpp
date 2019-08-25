#include "readings_worker.h"

#include "hal/hal.h"
#include "readings_taker.h"

namespace fk {

FK_DECLARE_LOGGER("rw");

void ReadingsWorker::run(WorkerContext &wc) {
    auto lock = storage_mutex.acquire(UINT32_MAX);

    auto started = fk_uptime();
    auto pool = MallocPool{ "readings", ModuleMemoryAreaSize };
    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();
    ModuleContext mc{ gs.get(), module_bus };
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory }; // Not opened!
    ModuleScanning scanning{ get_modmux() };
    ReadingsTaker readings_taker{ scanning, storage, get_modmux(), true };
    FK_ASSERT(readings_taker.take(mc, pool));

    memory.log_statistics();

    loginfo("done (pool = %zd/%zd bytes) (%" PRIu32 "ms)", pool.used(), pool.size(), fk_uptime() - started);
}

}
