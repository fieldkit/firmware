#include "networking/network_duration.h"
#include "config.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("network");

NetworkDuration::NetworkDuration() : seconds_{ FiveMinutesSeconds } {
    on_ = fk_uptime();
}

bool NetworkDuration::always_on() const {
    return seconds_ == UINT32_MAX;
}

bool NetworkDuration::on(uint32_t activity) const {
    if (always_on()) {
        return true;
    }

#if defined(FK_ENABLE_NETWORK_UP_AND_DOWN)
    auto seconds_up = (fk_uptime() - on_) / 1000;
    return seconds_up < OneMinuteSeconds * 2;
#else
    auto seconds_up = (fk_uptime() - activity) / 1000;
    return seconds_up < seconds_;
#endif
}

NetworkDuration NetworkDuration::operator=(uint32_t seconds) {
    seconds_ = std::max(seconds, OneMinuteSeconds);
    loginfo("network-duration: %" PRIu32, seconds_);
    return *this;
}

} // namespace fk
