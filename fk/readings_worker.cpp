#include <samd51_common.h>

#include "graceful_shutdown.h"
#include "hal/hal.h"
#include "readings_worker.h"
#include "state_manager.h"

#include "modules/scan_modules_worker.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool scan, bool read_only, ModulePowerState power_state)
    : scan_(scan), read_only_(read_only), power_state_(power_state) {
}

void ReadingsWorker::run(Pool &pool) {
    if (get_ipc()->has_running_worker(WorkerCategory::Polling)) {
        return;
    }

    if (!prepare(pool)) {
        return;
    }

    if (!take(pool)) {
        logerror("take");
        return;
    }

    if (!read_only_) {
        if (!save(pool)) {
            logerror("save");
            return;
        }
    }
}

bool ReadingsWorker::prepare(Pool &pool) {
    auto state = read_state();
    if (state.throttle) {
        logwarn("readings throttled");
        return false;
    }

    if (scan_ || !state.scanned) {
        ScanModulesWorker scan_worker;
        scan_worker.run(pool);
    }

    return true;
}

bool ReadingsWorker::take(Pool &pool) {
    // So, this is a little strange because we're getting a read only
    // lock but we do actually write the live readings. No real
    // danger, yet but it's strange.
    auto gs = get_global_state_ro();
    auto attached = gs.get()->dynamic.attached();
    if (attached == nullptr) {
        logerror("scan necessary");
        return false;
    }

    if (attached->take_readings(pool) < 0) {
        logerror("take readings");
        return false;
    }

    return true;
}

bool ReadingsWorker::save(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

    // jlewallen: storage-write
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    if (!storage.begin()) {
        return false;
    }

    auto gs = get_global_state_rw();

    auto meta_record_number = storage.meta_ops()->write_modules(gs.get(), &fkb_header, pool);
    if (!meta_record_number) {
        return false;
    }

    DataRecord record{ pool };
    record.include_readings(gs.get(), &fkb_header, *meta_record_number, pool);

    if (!storage.data_ops()->write_readings(gs.get(), &record.record(), pool)) {
        return false;
    }

    if (!storage.flush()) {
        return false;
    }

    return true;
}

ReadingsWorker::ThrottleAndScanState ReadingsWorker::read_state() {
    auto gs = get_global_state_rw();
    auto scanned = gs.get()->dynamic.attached() != nullptr;
    if (gs.get()->runtime.readings > 0) {
        auto elapsed = fk_uptime() - gs.get()->runtime.readings;
        if (elapsed < TenSecondsMs) {
            return ThrottleAndScanState{ true, scanned };
        }
    }
    gs.get()->runtime.readings = fk_uptime();
    return ThrottleAndScanState{ false, scanned };
}

} // namespace fk
