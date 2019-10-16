#pragma once

#include "platform.h"

namespace fk {

class Timer {
private:
    uint32_t started_;
    uint32_t duration_;

public:
    Timer() : started_(fk_uptime()), duration_(0) {
    }

    Timer(uint32_t duration) : started_(fk_uptime()), duration_(duration) {
    }

public:
    bool done(uint32_t duration) {
        return fk_uptime() - started_ > duration;
    }

    bool done() {
        FK_ASSERT(duration_ > 0);
        return done(duration_);
    }

};

}
