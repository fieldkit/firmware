#include "platform.h"
#include "hal/metal/udp_discovery.h"
#include "common.h"
#include "utilities.h"
#include "config.h"

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

bool UDPDiscovery::service() {
    if (!initialized_) {
        return true;
    }

    if (fk_uptime() > publish_) {
        loginfo("publishing");
        if (!udp_.beginPacket(IPAddress(224, 1, 2, 3), 22143)) {
            logerror("begin failed!");
        } else {
            fk_serial_number_t sn;
            char device_id_hex[64];
            bytes_to_hex_string(device_id_hex, sizeof(device_id_hex), (uint8_t *)&sn, sizeof(sn));
            udp_.write((uint8_t *)device_id_hex, strlen(device_id_hex));
            if (udp_.endPacket() != 0) {
                logerror("send failed!");
            }
        }
        publish_ = fk_uptime() + NetworkUdpDiscoveryInterval;
    }

    return true;
}

} // namespace fk

#endif
