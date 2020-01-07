#pragma once

#include <SEGGER_RTT.h>

#include <alogging/alogging.h>

#include "log_buffer.h"

namespace fk {

bool fk_logging_initialize();

bool fk_logging_dump_buffer();

log_buffer &fk_log_buffer();

bool fk_log_buffer_try_lock();

void fk_log_buffer_unlock();

class LogBufferLock {
private:
    bool success_{ false };

public:
    LogBufferLock() {
        success_ = fk_log_buffer_try_lock();
    }

    virtual ~LogBufferLock() {
        fk_log_buffer_unlock();
    }

    operator bool() {
        return success_;
    }
};

}
