#pragma once

#include <inttypes.h>
#include <string.h>

#define  optional_CONFIG_NO_EXCEPTIONS 1
#include <nonstd/optional.hpp>

#include "debugging.h"
#include "errors.h"
#include "logging.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Invoked to handle assertion failures. This is variadic so we can create more
 * useful messages, with interpolated values, etc...
 */
void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...);

void *fk_malloc_internal(size_t size, const char *file, int32_t line);

#define fk_malloc(size)    fk_malloc_internal(size, __FILE__, __LINE__)

void fk_free_internal(void *ptr, const char *file, int32_t line);

#define fk_free(ptr)       fk_free_internal(ptr, __FILE__, __LINE__)

/**
 * Zero a region of memory.
 */
#define memzero(ptr, size) memset(ptr, 0, size)

#ifdef __cplusplus
}
#endif

namespace fk {

using nonstd::optional;
using nonstd::nullopt;

/**
 * Halts execution if the given expression evaluates to false. See the fk_assert
 * function for more information.
 */
#define FK_ASSERT_INTERNAL(expression, f, ...)        (void)((expression) || (fk_assert(#expression, __FILE__, __LINE__, f, ##__VA_ARGS__), 0))

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
 * Check the return value of an I2C operation and return true if the operation
 * succeeded and false otherwise.
 */
#define I2C_CHECK(expr)                               ((expr) == 0)

/**
 * Declare loginfo, logerror, logwarn functions and any other necessary helpers
 * for a specific logger. Typically used at the top of a file.
 */
#define FK_DECLARE_LOGGER_MEMBERS(name)                                         \
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
 * Declare loginfo, logerror, logwarn functions and any other necessary helpers
 * for a specific logger. Typically used at the top of a file.
 */
#define FK_DECLARE_LOGGER(name)                                         \
    constexpr const char *LOG_FACILITY = name;                          \
    static void loginfo(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2)));    \
    static void logerror(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2)));   \
    static void logwarn(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2)));    \
    static void logtrace(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2)));   \
    static void logdebug(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2)));   \
    static void logverbose(const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 1, 2))); \
    FK_DECLARE_LOGGER_MEMBERS(name)

/**
 * Evaluates to 1 shifted by the given position.
 */
#define BIT(nr)               (1UL << (nr))

enum class Availability {
    Unknown,
    Available,
    Unavailable
};

template<typename T>
struct TypeName {
};

#define FK_ENABLE_TYPE_NAME(Type)    \
    template<>                       \
    struct TypeName<Type> {          \
        static const char *get() {   \
            return #Type;            \
        }                            \
    };

/**
 * RIAA-esque class for temporarily changing log level.
 */
class ScopedLogLevelChange {
private:
    uint8_t previous_;

public:
    explicit ScopedLogLevelChange(LogLevels level) {
        previous_ = log_get_level();
        log_configure_level(level);
    }

    virtual ~ScopedLogLevelChange() {
        log_configure_level((LogLevels)previous_);
    }
};

} // namespace fk
