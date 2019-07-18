#pragma once

#include "common.h"
#include "tiny_printf.h"

#include <Wire.h>
#include <RTClib.h>

namespace fk {

class CoreClock {
private:
    constexpr static uint8_t Address = 0x51u;

private:
    TwoWire *wire_;

public:
    CoreClock(TwoWire &wire);

public:
    bool begin();

    bool now(DateTime &now);

    bool adjust(DateTime now);

    DateTime now();

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

constexpr size_t MaximumLengthOfTimeString = length("0000/00/00 00:00:00");

inline void timeToString(char *buffer, size_t length, DateTime dt) {
    tiny_snprintf(buffer, length, "%d/%d/%d %02d:%02d:%02d",
                  dt.year(), dt.month(), dt.day(),
                  dt.hour(), dt.minute(), dt.second());
}

class FormattedTime {
private:
    char buffer_[MaximumLengthOfTimeString + 1];

public:
    FormattedTime(DateTime dt) {
        timeToString(buffer_, sizeof(buffer_), dt);
    }

    const char *cstr() const {
        return buffer_;
    }

};

}
