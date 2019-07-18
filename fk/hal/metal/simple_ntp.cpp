#include "platform.h"
#include "hal/metal/simple_ntp.h"

#if defined(ARDUINO)

#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("ntp");

constexpr uint32_t NtpSyncInterval = 24 * 60 * 1000 * 60;
constexpr uint32_t NtpRetryAfter = 2 * 1000;
constexpr uint64_t SeventyYears = 2208988800UL;
constexpr uint32_t PacketSize = 48;

SimpleNTP::SimpleNTP() {
}

SimpleNTP::~SimpleNTP() {
    stop();
}

bool SimpleNTP::service() {
    if (synced_ > 0 && fk_uptime() - synced_ < NtpSyncInterval) {
        return true;
    }

    if (queried_ == 0 || fk_uptime() - queried_ > NtpRetryAfter) {
        loginfo("asking for time...");
        start();
        queried_ = fk_uptime();
    }

    if (udp_.parsePacket()) {
        uint8_t buffer[PacketSize];

        udp_.read(buffer, sizeof(buffer));

        // Pull time from the packet. Stored as a DWORD here as seconds since 1/1/1900
        auto high = word(buffer[40], buffer[41]);
        auto low = word(buffer[42], buffer[43]);
        auto seconds_since_1900 = high << 16 | low;
        auto new_epoch = (uint32_t)(seconds_since_1900 - SeventyYears);

        CoreClock clock{ Wire };
        auto old_epoch = clock.now().unixtime();
        clock.adjust(new_epoch);

        FormattedTime new_formatted{ new_epoch };
        FormattedTime old_formatted{ old_epoch };
        loginfo("utc: '%s' -> '%s' (%" PRId64 "s)", old_formatted.cstr(), new_formatted.cstr(), (int64_t)new_epoch - old_epoch);

        synced_ = fk_uptime();

        stop();

        return true;
    }

    return true;
}

void SimpleNTP::start() {
    if (!initialized_) {
        udp_.begin(2390);
        initialized_ = true;
    }

    send();
}

void SimpleNTP::stop() {
    if (initialized_) {
        udp_.stop();
        initialized_ = false;
    }
}

bool SimpleNTP::send() {
    uint8_t buffer[PacketSize];

    bzero(buffer, sizeof(buffer));

    buffer[0] = 0b11100011;   // LI, Version, Mode
    buffer[1] = 0;            // Stratum, or type of clock
    buffer[2] = 6;            // Polling Interval
    buffer[3] = 0xEC;         // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    buffer[12]  = 49;
    buffer[13]  = 0x4E;
    buffer[14]  = 49;
    buffer[15]  = 52;

    udp_.beginPacket(addresses_[index_ % NumberOfAddresses], 123);
    udp_.write(buffer, sizeof(buffer));
    udp_.endPacket();

    index_++;

    return true;
}

}

#endif // defined(ARDUINO)
