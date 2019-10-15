#pragma once

#include "common.h"
#include "hal/board.h"
#include "tiny_printf.h"

#include <lwcron/lwcron.h>

namespace fk {

using DateTime = lwcron::DateTime;

class CoreClock {
private:
    constexpr static uint8_t Address = 0x51u;

private:
    bool configured_{ false };
    TwoWireWrapper wire_;

public:
    CoreClock();
    CoreClock(TwoWireWrapper wire);

public:
    bool begin();

    bool internal(DateTime &now);

    bool external(DateTime &now);

    bool adjust(DateTime now);

    bool sync();

    bool configure();

    DateTime now();

    DateTime get_external();

    bool valid();

public:
    void read_timestamp_registers();

    void clear_timestamp_registers();

private:
    void log_tsr(uint8_t *ts);

};

template<size_t N>
constexpr size_t length(char const (&)[N]) {
    return N - 1;
}

constexpr const char *TimeFormatReadable = "%d/%d/%d %02d:%02d:%02d";
constexpr const char *TimeFormatMachine = "%04d%02d%02d_%02d%02d%02d";
constexpr size_t MaximumLengthOfTimeString = length("0000/00/00 00:00:00");

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

}
