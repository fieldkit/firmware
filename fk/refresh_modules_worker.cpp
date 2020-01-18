#include "refresh_modules_worker.h"
#include "readings_worker.h"

namespace fk {

RefreshModulesWorker::RefreshModulesWorker() {
}

void RefreshModulesWorker::run(Pool &pool) {
    ReadingsWorker worker(true, true);
    worker.run(pool);
}

}
