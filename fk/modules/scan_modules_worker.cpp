#include "modules/scan_modules_worker.h"
#include "modules/scanning.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("modscan");

ScanModulesWorker::ScanModulesWorker() {
}

void ScanModulesWorker::run(Pool &pool) {
    auto lock = get_modmux()->lock();

    state::DynamicState dynamic;

    if (dynamic.attached()->create(pool) < 0) {
        logerror("scanning");
    }

    auto gs = get_global_state_rw();
    gs.get()->dynamic = std::move(dynamic);
}

} // namespace fk
