#pragma once

#include "common.h"
#include "config.h"

namespace fk {

class Pool;

struct GpsState {
public:
    uint8_t enabled{ 0 };
    uint8_t fix{ 0 };
    uint8_t satellites{ 0 };
    uint64_t time{ 0 };
    uint16_t hdop{ 0 };
    float longitude{ 0.0f };
    float latitude{ 0.0f };
    float altitude{ 0.0f };
    uint32_t chars{ 0 };
    uint32_t fixed_at{ 0 };

public:
    GpsState *clone(Pool &pool) const;
};

} // namespace fk