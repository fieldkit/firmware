#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class ReadingsWorker : public Worker {
private:
    bool scan_;
    bool read_only_;
    bool throttle_;
    ModulePowerState power_state_{ ModulePowerState::Unknown };
    StorageUpdate storage_update_;

public:
    ReadingsWorker(bool scan, bool read_only, bool throttle = true, ModulePowerState power_state = ModulePowerState::Unknown);

public:
    void run(Pool &pool) override;

    const char *name() const override {
        if (read_only_) {
            return "livedata";
        }
        return "readings";
    }

protected:
    bool prepare(Pool &pool);
    bool take(state::ReadingsListener *listener, Pool &pool);
    bool save(Pool &pool);
    bool update_global_state(Pool &pool);

private:
    struct ThrottleAndScanState {
        bool throttle;
        bool scanned;
    };

    ThrottleAndScanState read_state();
};

FK_ENABLE_TYPE_NAME(ReadingsWorker);

} // namespace fk
