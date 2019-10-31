#include <samd51_common.h>

#include "factory_wipe_worker.h"
#include "hal/hal.h"
#include "storage/storage.h"
#include "gs_progress_callbacks.h"
#include "factory_wipe.h"

namespace fk {

FK_DECLARE_LOGGER("factory-wipe");

void FactoryWipeWorker::run(Pool &pool) {
    GlobalStateProgressCallbacks progress;
    Storage storage{ MemoryFactory::get_data_memory() };
    FactoryWipe factory_wipe{ storage };

    if (!storage.begin()) {
        logwarn("error initializing storage");
    }

    if (factory_wipe.wipe(&progress)) {
        fk_delay(500);
        fk_restart();
    }
}

}
