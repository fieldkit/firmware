#pragma once

#include "readings_worker.h"

namespace fk {

class PollSensorsWorker : public ReadingsWorker {
private:
    uint32_t interval_{ TenSecondsMs };

public:
    explicit PollSensorsWorker(bool scan, bool read_only, uint32_t interval);

public:
    void run(Pool &pool) override;
};

FK_ENABLE_TYPE_NAME(PollSensorsWorker);

} // namespace fk
