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

bool fk_logs_flush();

void fk_logs_clear();

void fk_logs_printf(const char *f, ...);

void fk_logs_save();

void fk_logs_write_saved_and_free();

/**
 * Function for dumping memory in hexadecimal encoding to the console for debugging.
 */
void fk_logs_dump_memory(const char *prefix, uint8_t const *p, size_t size, ...);

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
