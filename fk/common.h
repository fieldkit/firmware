#pragma once

#include <inttypes.h>
#include <alogging/alogging.h>
#include <string.h>

namespace fk {

/**
 * Halts execution if the given expression evaluates to false. See the fk_assert
 * function for more information.
 */
#define FK_ASSERT(expression)                         (void)((expression) || (__cfy_assert(#expression, __FILE__, __LINE__), 0))

/**
 * Write hex representation of bytes to a separate buffer.
 */
size_t bytes_to_hex_string(char *buffer, size_t buffer_size, const uint8_t *data, size_t data_size);

/**
 * Check the return value of an I2C operation and return true if the operation
 * succeeded and false otherwise.
 */
#define I2C_CHECK(expr)                               ((expr) == 0)

/**
 * Declare loginfo, logerror, logwarn functions and any other necessary helpers
 * for a specific logger. Typically used at the top of a file.
 */
#define FK_DECLARE_LOGGER(name)                  \
    constexpr const char *LOG_FACILITY = name;                          \
    static void loginfo(const char *f, ...) __attribute__((unused));    \
    static void logerror(const char *f, ...) __attribute__((unused));   \
    static void logwarn(const char *f, ...) __attribute__((unused));    \
    static void logtrace(const char *f, ...) __attribute__((unused));   \
    static void logdebug(const char *f, ...) __attribute__((unused));   \
    static void logverbose(const char *f, ...) __attribute__((unused));   \
    static void loginfo(const char *f, ...) {    \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::INFO, name, f, args);  \
        va_end(args);                            \
    }                                            \
    static void logerror(const char *f, ...) {   \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::ERROR, name, f, args); \
        va_end(args);                            \
    }                                            \
    static void logdebug(const char *f, ...) {   \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::DEBUG, name, f, args); \
        va_end(args);                            \
    }                                            \
    static void logtrace(const char *f, ...) {   \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::TRACE, name, f, args); \
        va_end(args);                            \
    }                                            \
    static void logverbose(const char *f, ...) { \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::TRACE, name, f, args); \
        va_end(args);                            \
    }                                            \
    static void logwarn(const char *f, ...) {    \
        va_list args;                            \
        va_start(args, f);                       \
        valogf(LogLevels::WARN, name, f, args);  \
        va_end(args);                            \
    }

/**
 * Evaluates to 1 shifted by the given position.
 */
#define BIT(nr)               (1UL << (nr))

}
