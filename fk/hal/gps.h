#pragma once

#include "common.h"

namespace fk {

struct GpsFix {
    bool valid;
    uint16_t satellites;
    uint32_t time;
    float longitude;
    float latitude;
    float altitude;
    uint16_t hdop;
    uint32_t chars;
    uint16_t good;
    uint16_t failed;
};

class Gps {
public:
    virtual bool begin() = 0;
    virtual bool check() = 0;
    virtual bool service(GpsFix &fix) = 0;
    virtual bool stop() = 0;

};

Gps *get_gps();

}
