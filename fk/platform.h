#pragma once

#include "common.h"

#if !defined(__SAMD51__)
#include <vector>
#endif

namespace fk {

typedef struct fk_serial_number_t {
    uint32_t dwords[4];

    fk_serial_number_t();
} fk_serial_number_t;

/**
 * Return the number of milliseconds since the hardware was started.
 */
uint32_t fk_uptime();

uint32_t fk_delay(uint32_t ms);

uint32_t fk_free_memory();

#if !defined(__SAMD51__)
uint32_t fk_fake_uptime(std::vector<uint32_t> more);
#endif

uint32_t fk_serial_number_get(fk_serial_number_t *sn);

class IntervalTimer {
private:
    uint32_t mark_;

public:
    IntervalTimer() : mark_(fk_uptime()) {
    }

    IntervalTimer(bool enabled) : mark_(enabled ? fk_uptime() : 0) {
    }

public:
    void mark() {
        mark_ = fk_uptime();
    }

    void disable() {
        mark_ = 0;
    }

    bool enabled() const {
        return mark_ > 0;
    }

    bool expired(uint32_t interval) {
        if (!enabled()) {
            return false;
        }
        auto now = fk_uptime();
        auto elapsed = now - mark_;
        if (elapsed >= interval) {
            if (mark_ + interval < now) {
                mark_ = now;
            }
            else {
                mark_ = now;
            }
            return true;
        }
        return false;
    }
};

}
