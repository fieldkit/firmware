#pragma once

#include "common.h"

namespace fk {

struct BatteryReading {
    bool available;
    float bus_voltage;
    float shunted_voltage;
    float ma;
    float mw;
};

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual BatteryReading get() = 0;

};

extern BatteryGauge *get_battery_gauge();

}
