#include "readings_worker.h"

#include "modules/refresh_modules_worker.h"
#include "modules/scan_modules_worker.h"

namespace fk {

RefreshModulesWorker::RefreshModulesWorker(bool readings) : readings_(readings) {
}

void RefreshModulesWorker::run(Pool &pool) {
    if (readings_) {
        ReadingsWorker worker{ true, true, true, ModulePowerState::Unknown };
        worker.run(pool);
    } else {
        ScanModulesWorker scan_worker;
        scan_worker.run(pool);
    }
}

} // namespace fk
