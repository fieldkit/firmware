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

#define FK_DISABLE_IRQ     SEGGER_RTT_LOCK

#define FK_ENABLE_IRQ      SEGGER_RTT_UNLOCK

/**
 * These disappeared on after upgrading compilers.
 */
#if !defined(PRIu64)
#define PRIu64             "llu"
#define PRId64             "lld"
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

#if defined(__SAMD51__)
#define FK_ASSERT_ADDRESS(ptr)                        FK_ASSERT((intptr_t)ptr != 0 && (intptr_t)ptr >= 0x20000000 && (intptr_t)ptr < 0x20040000);
#else
#define FK_ASSERT_ADDRESS(ptr)                        FK_ASSERT((intptr_t)ptr != 0);
#endif

/**
 * Check the return value of an I2C operation and return true if the operation
 * succeeded and false otherwise.
 */
#define I2C_CHECK(expr)                               ((expr) == 0)

/**
 * Declare logger facility to be used for logging calls in this scope.
 */
#define FK_DECLARE_LOGGER(name)        constexpr const char *LOG_FACILITY = name;

/**
 * Declare logger facility to be used for logging calls in this class.
 */
#define FK_DECLARE_LOGGER_MEMBER(name) static constexpr const char *LOG_FACILITY = name;

#define loginfo(f, ...)                fk_logf(LogLevels::INFO, LOG_FACILITY, f, ## __VA_ARGS__)
#define logwarn(f, ...)                fk_logf(LogLevels::WARN, LOG_FACILITY, f, ## __VA_ARGS__)
#define logerror(f, ...)               fk_logf(LogLevels::ERROR, LOG_FACILITY, f, ## __VA_ARGS__)
#define logdebug(f, ...)               fk_logf(LogLevels::DEBUG, LOG_FACILITY, f, ## __VA_ARGS__)
#define logtrace(f, ...)               fk_logf(LogLevels::TRACE, LOG_FACILITY, f, ## __VA_ARGS__)
#define logverbose(f, ...)             fk_logf(LogLevels::VERBOSE, LOG_FACILITY, f, ## __VA_ARGS__)

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
