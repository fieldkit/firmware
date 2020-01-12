#pragma once

#include "worker.h"
#include "readings.h"
#include "storage/storage.h"

namespace fk {

class ReadingsWorker {
private:
    bool read_only_;
    FileHeader meta_fh_;
    FileHeader data_fh_;

public:
    ReadingsWorker(bool read_only);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        if (read_only_) {
            return "livedata";
        }
        return "readings";
    }

private:
    bool should_throttle();

    tl::expected<TakenReadings, Error> take_readings(Pool &pool);

};

FK_ENABLE_TYPE_NAME(ReadingsWorker);

}
