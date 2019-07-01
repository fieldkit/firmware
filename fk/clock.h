#pragma once

#include "common.h"

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

    void adjust(DateTime now);

    DateTime now();

public:
    void read_timestamp_registers();

    void clear_timestamp_registers();

private:
    void log_tsr(uint8_t *ts);

};

}
