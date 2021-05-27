#include <samd51_common.h>

#include "factory_wipe_worker.h"
#include "hal/hal.h"
#include "storage/storage.h"
#include "gs_progress_callbacks.h"
#include "factory_wipe.h"
#include "graceful_shutdown.h"

namespace fk {

FK_DECLARE_LOGGER("factory-wipe");

FactoryWipeWorker::FactoryWipeWorker(bool restart) : restart_(restart) {
}

void FactoryWipeWorker::run(Pool &pool) {
    if (!initialize_memory_if_necessary()) {
        logerror("memory failed");
        return;
    }

    auto memory = MemoryFactory::get_data_memory();
    GlobalStateProgressCallbacks progress;
    Storage storage{ memory, pool };
    FactoryWipe factory_wipe{ storage };

    if (factory_wipe.wipe(&progress)) {
        if (restart_) {
            fk_graceful_shutdown();
            fk_restart();
        }
    }
}

bool FactoryWipeWorker::initialize_memory_if_necessary() {
    auto banks = MemoryFactory::get_data_memory_banks();
    auto memory = MemoryFactory::get_data_memory();

    auto nbanks = 0u;
    for (auto i = 0u; i < MemoryFactory::NumberOfDataMemoryBanks; ++i) {
        auto &bank = *banks[i];

        // TODO: Why is this necessary?
        fk_delay(100);

        if (bank.begin()) {
            loginfo("memory bank #%d... OK", nbanks);
            nbanks++;
        }
    }

    if (!memory->begin()) {
        return false;
    }

    return true;
}

}
