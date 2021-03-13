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

void fk_logs_saved_capture();

void fk_logs_saved_write(bool echo);

void fk_logs_saved_free();

void fk_log_debugging(const char *source);

/**
 * Function for dumping memory in hexadecimal encoding to the console for debugging.
 */
void fk_logs_dump_memory(const char *prefix, uint8_t const *p, size_t size, ...);

/**
 * Main logging function, the workhorse. This is called everywhere,
 * usually by macro to add the logger and level.
 */
void fk_logf(LogLevels level, const char *logger, const char *f, ...);

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
