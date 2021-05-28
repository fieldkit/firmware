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

class IntervalTimer {
private:
    uint32_t interval_;
    uint32_t mark_;

public:
    IntervalTimer(int32_t interval) : interval_(interval), mark_(fk_uptime()) {
    }

    IntervalTimer(uint32_t interval) : interval_(interval), mark_(fk_uptime()) {
    }

    IntervalTimer(bool enabled) : interval_(0), mark_(enabled ? fk_uptime() : 0) {
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

    uint32_t interval() const {
        return interval_;
    }

    uint32_t ms_left() const {
        return ms_left(interval_);
    }

    uint32_t ms_left(uint32_t interval) const {
        if (!enabled()) {
            return UINT32_MAX;
        }
        auto now = fk_uptime();
        auto elapsed = mark_ - now;
        if (elapsed >= interval) {
            return 0;
        }
        return interval - elapsed;
    }

    bool expired() {
        return expired(interval_);
    }

    bool expired(uint32_t interval) {
        if (!enabled()) {
            return false;
        }

        auto now = fk_uptime();
        auto elapsed = now - mark_;
        if (elapsed < interval) {
            return false;
        }

        if (mark_ + interval < now) {
            mark_ = now;
        } else {
            mark_ = now;
        }
        return true;
    }

};

} // namespace fk
