#pragma once

#include "common.h"

namespace fk {

struct MeterReading {
    bool available;
    float bus_voltage;
    float shunted_voltage;
    float ma;
    float mw;
};

struct PowerReading {
    bool available;
    bool charging;
    MeterReading battery;
    MeterReading solar;
};

struct ChargingStatus {
    uint32_t ticks;
    uint32_t blinks;
};

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual PowerReading get() = 0;

    virtual ChargingStatus status() = 0;

    virtual bool battery_available() = 0;

    virtual bool solar_available() = 0;

};

extern BatteryGauge *get_battery_gauge();

}
