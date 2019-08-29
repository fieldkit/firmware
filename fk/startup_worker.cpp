#include "startup_worker.h"
#include "storage/storage.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "tasks/tasks.h"

namespace fk {

StartupWorker::StartupWorker() {
}

StartupWorker::StartupWorker(Pool *pool) {
}

void StartupWorker::run(Pool &pool) {
    NoopSelfCheckCallbacks noop_callbacks;
    SelfCheck self_check(get_display(), get_network(), get_modmux());
    self_check.check(SelfCheckSettings{ }, noop_callbacks);

    Storage storage{ MemoryFactory::get_data_memory() };
    FactoryWipe fw{ get_buttons(), &storage };
    FK_ASSERT(fw.wipe_if_necessary());

    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
}

}
