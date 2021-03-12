#include "readings_worker.h"
#include "modules/refresh_modules_worker.h"

namespace fk {

RefreshModulesWorker::RefreshModulesWorker() {
}

void RefreshModulesWorker::run(Pool &pool) {
    ReadingsWorker worker{ true, true, ModulePowerState::Unknown };
    worker.run(pool);
}

}
