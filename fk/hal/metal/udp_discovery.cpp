#include "hal/metal/udp_discovery.h"
#include "common.h"
#include "config.h"
#include "platform.h"
#include "utilities.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("udp");

UDPDiscovery::UDPDiscovery() {
}

UDPDiscovery::~UDPDiscovery() {
    stop();
}

bool UDPDiscovery::start() {
    if (initialized_) {
        return true;
    }

    if (!udp_.beginMulticast(IPAddress(224, 1, 2, 3), 22143)) {
        return false;
    }

    initialized_ = true;
    publish_ = 0;

    return true;
}

void UDPDiscovery::stop() {
    if (initialized_) {
        udp_.stop();
        initialized_ = false;
    }
}

bool UDPDiscovery::service(Pool *pool) {
    if (!initialized_) {
        return true;
    }

    if (fk_uptime() > publish_) {
        send();
        publish_ = fk_uptime() + NetworkUdpDiscoveryInterval;
    }

    return true;
}

bool UDPDiscovery::send() {
    loginfo("publishing");
    if (!udp_.beginPacket(IPAddress(224, 1, 2, 3), NetworkUdpDiscoveryPort)) {
        logerror("begin failed!");
        return false;
    }

    fk_serial_number_t sn;
    udp_.write((uint8_t *)&sn, sizeof(sn));
    if (udp_.endPacket() != 0) {
        logerror("send failed!");
        return false;
    }

    return true;
}

} // namespace fk

#endif
