#pragma once

#include "common.h"

namespace fk {

struct GpsFix {
    bool valid{ false };
    uint16_t satellites{ 0 };
    uint32_t time{ 0 };
    float longitude{ 1000.0f };
    float latitude{ 1000.0f };
    float altitude{ 1000.0f };
    uint16_t hdop{ 0 };
    uint32_t chars{ 0 };
    uint16_t good{ 0 };
    uint16_t failed{ 0 };
    uint32_t position_fix_age{ 0 };
    uint32_t time_fix_age{ 0 };
};

class Gps {
public:
    virtual bool begin() = 0;
    virtual bool service(GpsFix &fix) = 0;
    virtual bool stop() = 0;

};

Gps *get_gps();

}
