#pragma once

#include "common.h"

namespace fk {

struct BatteryReading {
    bool available;
    int32_t cellv;
    int32_t soc;
    int32_t temp;
    float tte;
    float ttf;
};

class BatteryGauge {
public:
    virtual bool begin() = 0;

    virtual BatteryReading get() = 0;

};

extern BatteryGauge *get_battery_gauge();

}
