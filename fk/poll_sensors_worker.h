#pragma once

#include "readings_worker.h"

namespace fk {

class PollSensorsWorker : public ReadingsWorker {
private:
    uint32_t interval_{ TenSecondsMs };

public:
    explicit PollSensorsWorker(bool scan, bool read_only, bool throttle, uint32_t interval);

public:
    void run(Pool &pool) override;

private:
    void before_readings(Pool &pool);
    void take_readings();
};

FK_ENABLE_TYPE_NAME(PollSensorsWorker);

} // namespace fk
