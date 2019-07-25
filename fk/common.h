#pragma once

#include <inttypes.h>
#include <alogging/alogging.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Invoked to handle assertion failures. This is variadic so we can create more
 * useful messages, with interpolated values, etc...
 */
void __fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...);

#ifdef __cplusplus
}
#endif

namespace fk {

/**
 * Halts execution if the given expression evaluates to false. See the fk_assert
 * function for more information.
 */
#define FK_ASSERT_INTERNAL(expression, f, ...)        (void)((expression) || (__fk_assert(#expression, __FILE__, __LINE__, f, ##__VA_ARGS__), 0))

/**
 * Halts execution if the given expression evaluates to false. See the fk_assert
 * function for more information.
 */
#define FK_ASSERT(expression)                         FK_ASSERT_INTERNAL(expression, "")

/**
 *
 */
#define FK_ASSERT_EQ(a, b)                            FK_ASSERT_INTERNAL((a) == (b), "%d == %d", a, b)

/**
 *
 */
#define FK_ASSERT_NE(a, b)                            FK_ASSERT_INTERNAL((a) != (b), "%d != %d", a, b)

/**
 *
 */
#define FK_ASSERT_LE(a, b)                            FK_ASSERT_INTERNAL((a) <= (b), "%d <= %d", a, b)

/**
 *
 */
#define FK_ASSERT_LT(a, b)                            FK_ASSERT_INTERNAL((a) < (b), "%d < %d", a, b)

/**
 *
 */
#define FK_ASSERT_GE(a, b)                            FK_ASSERT_INTERNAL((a) >= (b), "%d >= %d", a, b)

/**
 *
 */
#define FK_ASSERT_GT(a, b)                            FK_ASSERT_INTERNAL((a) > (b), "%d > %d", a, b)

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
#define FK_DECLARE_LOGGER(name)                                         \
    constexpr const char *LOG_FACILITY = name;                          \
    static void loginfo(const char *f, ...) __attribute__((unused));    \
    static void logerror(const char *f, ...) __attribute__((unused));   \
    static void logwarn(const char *f, ...) __attribute__((unused));    \
    static void logtrace(const char *f, ...) __attribute__((unused));   \
    static void logdebug(const char *f, ...) __attribute__((unused));   \
    static void logverbose(const char *f, ...) __attribute__((unused)); \
    static void loginfo(const char *f, ...) {      \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::INFO, name, f, args);    \
        va_end(args);                              \
    }                                              \
    static void logerror(const char *f, ...) {     \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::ERROR, name, f, args);   \
        va_end(args);                              \
    }                                              \
    static void logdebug(const char *f, ...) {     \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::DEBUG, name, f, args);   \
        va_end(args);                              \
    }                                              \
    static void logtrace(const char *f, ...) {     \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::TRACE, name, f, args);   \
        va_end(args);                              \
    }                                              \
    static void logverbose(const char *f, ...) {   \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::VERBOSE, name, f, args); \
        va_end(args);                              \
    }                                              \
    static void logwarn(const char *f, ...) {      \
        va_list args;                              \
        va_start(args, f);                         \
        valogf(LogLevels::WARN, name, f, args);    \
        va_end(args);                              \
    }

/**
 * Evaluates to 1 shifted by the given position.
 */
#define BIT(nr)               (1UL << (nr))

}
