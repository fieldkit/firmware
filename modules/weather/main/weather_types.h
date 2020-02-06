#pragma once

#include <lwcron/lwcron.h>

#include "modules/shared/crc.h"

namespace fk {

static constexpr float RainPerTick = 0.2794; // mm
static constexpr float WindPerTick = 2.4; // km/hr

using DateTime = lwcron::DateTime;

struct WindDirection {
    int16_t raw{ -1 };
    int16_t angle{ -1 };

    WindDirection() {
    }

    WindDirection(int16_t raw, int16_t angle) : raw(raw), angle(angle) {
    }
};

struct WindReading {
    float speed{ 0 };
    WindDirection direction;

    WindReading() {
    }

    WindReading(float speed, WindDirection direction) : speed(speed), direction(direction) {
    }

    bool stronger_than(WindReading const &r) const {
        return speed > r.speed;
    }
};

template<typename T>
static uint32_t calculate_crc(uint32_t seed, T &object) {
    return crc32_checksum(seed, (uint8_t *)&object, sizeof(T) - sizeof(uint32_t));
}

}
