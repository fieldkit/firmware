#pragma once

#include "common.h"
#include "hal/board.h"
#include "tiny_printf.h"

#include <lwcron/lwcron.h>

namespace fk {

using DateTime = lwcron::DateTime;

class CoreClock {
private:
    bool configured_{ false };

public:
    explicit CoreClock();

public:
    bool begin();

    bool internal(DateTime &now);

    bool external(DateTime &now);

    bool adjust(DateTime now);

    bool sync();

    bool configure();

    DateTime now();

    DateTime get_external();

    void compare();

private:
    bool adjust_internal(DateTime now);
};

template <size_t N> constexpr size_t length(char const (&)[N]) {
    return N - 1;
}

template <typename T> constexpr T const &constexpr_max(T const &a, T const &b) {
    return a > b ? a : b;
}

constexpr const char *TimeFormatReadable = "%d/%d/%d %02d:%02d:%02d";
constexpr size_t MaximumLengthOfReadableTimeString = length("0000/00/00 00:00:00");
constexpr const char *TimeFormatMachine = "%04d%02d%02d_%02d%02d%02d";
constexpr size_t MaximumLengthOfMachineTimeString = length("00000000_000000");
constexpr const char *TimeFormatLogs = "%04d%02d%02d_%02d%02d";
constexpr size_t MaximumLengthOfLogsTimeString = length("00000000_0000");
constexpr size_t MaximumLengthOfTimeString =
    constexpr_max(constexpr_max(MaximumLengthOfReadableTimeString, MaximumLengthOfMachineTimeString), MaximumLengthOfLogsTimeString);

class FormattedTime {
public:
private:
    char buffer_[MaximumLengthOfTimeString + 1];

public:
    FormattedTime(DateTime dt, const char *f = TimeFormatReadable) {
        time_to_string(f, buffer_, sizeof(buffer_), dt);
    }

    const char *cstr() const {
        return buffer_;
    }

private:
    static void time_to_string(const char *f, char *buffer, size_t length, DateTime dt) {
        tiny_snprintf(buffer, length, f, dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());
    }
};

CoreClock *get_clock();

uint32_t get_clock_now();

uint32_t clock_adjust(uint32_t new_epoch);

uint32_t clock_adjust_maybe(uint32_t new_epoch);

} // namespace fk
