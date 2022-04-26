#include <samd51_common.h>

#include "graceful_shutdown.h"
#include "hal/hal.h"
#include "readings_worker.h"
#include "state_manager.h"
#include "clock.h"

#include "modules/scan_modules_worker.h"
#include "update_readings_listener.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("rw");

ReadingsWorker::ReadingsWorker(bool scan, bool read_only, bool throttle, ModulePowerState power_state)
    : scan_(scan), read_only_(read_only), throttle_(throttle), power_state_(power_state) {
}

void ReadingsWorker::run(Pool &pool) {
    if (get_ipc()->has_running_worker(WorkerCategory::Polling)) {
        return;
    }

    auto state = read_state();
    if (throttle_ && state.throttle) {
        logwarn("readings throttled");
        return;
    }

    if (scan_ || !state.scanned) {
        loginfo("scanning");

        if (!scan(pool)) {
            return;
        }
    } else {
        loginfo("scan skipped");
    }

    UpdateReadingsListener listener{ pool };
    if (!take(&listener, pool)) {
        logerror("take");
        return;
    }

    FK_ASSERT(listener.flush() >= 0);

    if (!read_only_) {
        if (!save(pool)) {
            logerror("save");
            return;
        }

        update_global_state(pool);
    }
}

bool ReadingsWorker::scan(Pool &pool) {
    ScanModulesWorker scan_worker;
    scan_worker.run(pool);

    return true;
}

bool ReadingsWorker::take(state::ReadingsListener *listener, Pool &pool) {
    // So, this is a little strange because we're getting a read only
    // lock but we do actually write the live readings. No real
    // danger, yet but it's strange.
    auto gs = get_global_state_ro();
    auto attached = gs.get()->dynamic.attached();

    if (attached->take_readings(listener, pool) < 0) {
        logerror("take readings");
        return false;
    }

    return true;
}

bool ReadingsWorker::save(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    FK_ASSERT(lock);

    // This makes testing way easier and honestly, so far having the phylum logs
    // be verbose hasn't been necessary. I'd love a way to keep DEBUG and only
    // flush on certain conditions.
    ScopedLogLevelChange temporary_info_only{ LogLevels::INFO };

    // jlewallen: storage-write
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    if (!storage.begin()) {
        return false;
    }

    auto gs = get_global_state_ro();

    MetaRecord meta_record{ pool };
    if (!meta_record.include_modules(gs.get(), &fkb_header, pool)) {
        logwarn("include-modules failed, skipping write");
        return true;
    }

    auto meta_ops = storage.meta_ops();
    auto meta_record_number = meta_ops->write_record(SignedRecordKind::Modules, meta_record.record(), pool);
    if (!meta_record_number) {
        return false;
    }

    auto meta_attributes = meta_ops->attributes(pool);
    if (!meta_attributes) {
        return false;
    }

    DataRecord data_record{ pool };
    data_record.include_readings(gs.get(), &fkb_header, *meta_record_number, pool);

    auto data_ops = storage.data_ops();
    auto data_record_number = data_ops->write_readings(&data_record.record(), pool);
    if (!data_record_number) {
        return false;
    }

    auto data_attributes = data_ops->attributes(pool);
    if (!data_attributes) {
        return false;
    }

    storage_update_ = StorageUpdate{ .meta = StorageStreamUpdate{ meta_attributes->size, meta_attributes->records },
                                     .data = StorageStreamUpdate{ data_attributes->size, data_attributes->records },
                                     .nreadings = data_attributes->nreadings,
                                     .installed = storage.installed(),
                                     .used = storage.used(),
                                     .time = get_clock_now() };

    if (!storage.flush()) {
        return false;
    }

    return true;
}

bool ReadingsWorker::update_global_state(Pool &pool) {
    auto gs = get_global_state_rw();

    gs.get()->apply(storage_update_);

    return true;
}

ReadingsWorker::ThrottleAndScanState ReadingsWorker::read_state() {
    auto gs = get_global_state_rw();
    auto attached = gs.get()->dynamic.attached();
    auto scanned = attached != nullptr && attached->initialized();
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
