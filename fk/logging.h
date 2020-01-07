#pragma once

#include <SEGGER_RTT.h>

#include <alogging/alogging.h>

#include "log_buffer.h"

namespace fk {

bool fk_logging_initialize();

bool fk_logging_dump_buffer();

log_buffer &fk_log_buffer();

class RttLock {
private:

public:
    RttLock() {
    }

    virtual ~RttLock() {
    }
};

}
