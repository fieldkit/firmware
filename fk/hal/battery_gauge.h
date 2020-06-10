#pragma once

#include "common.h"

namespace fk {

struct Ina219Reading {
    bool available;
    float bus_voltage;
    float shunted_voltage;
    float ma;
    float mw;
};

struct BatteryReading {
    bool available;
    bool charging;
    Ina219Reading battery;
    Ina219Reading solar;
};

struct ChargingStatus {
    uint32_t ticks;
    uint32_t blinks;
};

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual BatteryReading get() = 0;

    virtual ChargingStatus status() = 0;

    virtual bool available() = 0;

};

extern BatteryGauge *get_battery_gauge();

}
