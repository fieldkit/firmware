#pragma once

#include <inttypes.h>
#include <alogging/alogging.h>

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
 * This is the theorhetical maximum number of modules that can be physically
 * connected to a single station. This is governed by the largest backplanes and
 * their possible combinations.
 */
constexpr size_t MaximumNumberOfModules = 8;

/**
 * The number of fixed HTTP routes that the firmware registers. These are routes
 * that are always there and never change with configuration.
 */
constexpr size_t HttpFixedRoutes = 2;

/**
 * The maximum number of HTTP routes that can be registered. Trying to register
 * more than this will fail. This is a function of the maximum number of modules
 * and the number of fixed routes.
 */
constexpr size_t HttpMaximumRoutes = MaximumNumberOfModules + HttpFixedRoutes;

/**
 * The maximum length of an incoming URL.
 */
constexpr size_t HttpdMaximumUrlLength = 64;

/**
 * How long to wait for a WiFi connection to establish.
 */
constexpr uint32_t WifiConnectionTimeoutMs = 30 * 1000;

/**
 * Buffer size for storing GPS sentences for debugging purposes.
 */
constexpr size_t GpsDebuggingBuffer = 64;

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
