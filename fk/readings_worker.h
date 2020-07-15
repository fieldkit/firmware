#pragma once

#include "worker.h"
#include "readings.h"
#include "storage/storage.h"

namespace fk {

class ReadingsWorker : public Worker {
private:
    bool scan_;
    bool read_only_;
    bool verify_;

public:
    ReadingsWorker(bool scan, bool read_only, bool verify);

public:
    void run(Pool &pool) override;

    uint8_t priority() const  override{
        return OS_PRIORITY_NORMAL;
    }

    const char *name() const override{
        if (read_only_) {
            return "livedata";
        }
        return "readings";
    }

private:
    struct ThrottleAndPowerSave {
        bool throttle;
        bool power_save;
    };

    ThrottleAndPowerSave read_throttle_and_power_save();

    tl::expected<TakenReadings, Error> take_readings(Pool &pool);


};

FK_ENABLE_TYPE_NAME(ReadingsWorker);

}
