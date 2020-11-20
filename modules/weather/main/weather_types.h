#pragma once

#include <lwcron/lwcron.h>

#include "modules/shared/crc.h"
#include "weather.h"

namespace fk {

static constexpr float RainPerTick = 0.336; // mm
static constexpr float WindPerTick = 2.4;   // km/hr

using DateTime = lwcron::DateTime;

struct WindDirection {
    int16_t raw{ -1 };
    int16_t angle{ -1 };

    WindDirection() {
    }

    WindDirection(int16_t raw, int16_t angle) : raw(raw), angle(angle) {
    }

    WindDirection(uint32_t raw_adc) : raw(get_adc_value_from_raw_adc(raw_adc)), angle(get_angle_from_raw_adc(raw_adc)) {
    }

    static int16_t get_adc_value_from_raw_adc(uint32_t raw_adc);

    static int16_t get_angle_from_raw_adc(uint32_t raw_adc);

    static float get_mv_from_raw_adc(uint32_t raw_adc);
};

struct WindReading {
public:
    float speed{ 0 };
    WindDirection direction;

public:
    WindReading() {
    }

    explicit WindReading(fk_wind_t raw) : speed{ raw.ticks * WindPerTick }, direction{ raw.direction } {
    }

    explicit WindReading(float speed, WindDirection direction) : speed(speed), direction(direction) {
    }

public:
    bool stronger_than(WindReading const &r) const {
        return speed > r.speed;
    }

    template<typename T, size_t Size>
    static WindReading get_average(T(&readings)[Size]) {
        int16_t mv = 0;
        auto speed_sum = 0.0f;
        auto number_of_samples = 0;
        auto direction_sum = readings[0].direction.angle;
        auto d = readings[0].direction.angle;
        for (auto i = 1 ; i < Size; i++) {
            if (readings[i].direction.angle != -1) {
                auto delta = readings[i].direction.angle - d;

                if (delta < -180) {
                    d += delta + 360;
                }
                else if (delta > 180) {
                    d += delta - 360;
                }
                else {
                    d += delta;
                }

                direction_sum += d;

                speed_sum += readings[i].speed;

                number_of_samples++;

                mv = readings[i].direction.raw;
            }
        }

        auto average_speed = speed_sum / (float)number_of_samples;

        auto average_direction = (int16_t)(direction_sum / number_of_samples);
        if (average_direction >= 360) {
            average_direction -= 360;
        }
        if (average_direction < 0) {
            average_direction += 360;
        }

        return WindReading{ average_speed, WindDirection{ mv, average_direction } };
    }

};

template<typename T>
static uint32_t calculate_crc(uint32_t seed, T &object) {
    return crc32_checksum(seed, (uint8_t *)&object, sizeof(T) - sizeof(uint32_t));
}

} // namespace fk
