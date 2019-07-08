#pragma once

#include "common.h"

namespace fk {

struct GpsFix {
    uint32_t satellites;
    uint32_t time;
    float longitude;
    float latitude;
    float altitude;
};

class Gps {
public:
    virtual bool begin() = 0;
    virtual bool check() = 0;
    virtual bool service(GpsFix &fix) = 0;

};

Gps *get_gps();

}
