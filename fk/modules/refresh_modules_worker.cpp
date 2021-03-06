#include "readings_worker.h"
#include "modules/refresh_modules_worker.h"

namespace fk {

RefreshModulesWorker::RefreshModulesWorker() {
}

void RefreshModulesWorker::run(Pool &pool) {
    ReadingsWorker worker{ true, true, false };
    worker.run(pool);
}

}
