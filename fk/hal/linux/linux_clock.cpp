#if !defined(__SAMD51__)

#include "config.h"
#include "hal/clock.h"

#include <ctime>

namespace fk {

static CoreClock clock;

CoreClock::CoreClock() {
}

bool CoreClock::begin() {
    return false;
}

void CoreClock::compare() {
}

CoreClock *get_clock() {
    return &clock;
}

uint32_t get_clock_now() {
    return std::time(0);
}

uint32_t clock_adjust(uint32_t new_epoch) {
    return new_epoch;
}

} // namespace fk

#endif