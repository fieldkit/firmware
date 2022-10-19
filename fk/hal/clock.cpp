#include "config.h"
#include "hal/clock.h"

namespace fk {

uint32_t clock_adjust_maybe(uint32_t new_epoch) {
    auto existing = get_clock_now();
    auto difference = std::abs((long)(existing - new_epoch));
    if (difference > AcceptableTimeDriftSeconds) {
        return clock_adjust(new_epoch);
    }
    return existing;
}

} // namespace fk
