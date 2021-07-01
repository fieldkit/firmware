#pragma once

#include "readings_worker.h"

namespace fk {

class PollSensorsWorker : public ReadingsWorker {
public:
    explicit PollSensorsWorker(bool scan, bool read_only);

public:
    void run(Pool &pool) override;
};

FK_ENABLE_TYPE_NAME(PollSensorsWorker);

} // namespace fk
