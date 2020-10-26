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
        if (pool_ != nullptr) {
            for (auto i = 0; i < 3; ++i) {
                send(fk_app_UdpStatus_UDP_STATUS_BYE, pool_);
                delay(50);
            }
        }
        else {
            logerror("missing pool");
        }
        udp_.stop();
        initialized_ = false;
    }
}

bool UDPDiscovery::service(Pool *pool) {
    if (!initialized_) {
        return true;
    }

    if (fk_uptime() > publish_) {
        send(fk_app_UdpStatus_UDP_STATUS_ONLINE, pool);
        publish_ = fk_uptime() + NetworkUdpDiscoveryInterval;
    }

    return true;
}

bool UDPDiscovery::send(fk_app_UdpStatus status, Pool *pool) {
    fk_serial_number_t sn;
    auto device_id_data = pool->malloc_with<pb_data_t>({
        .length = sizeof(sn),
        .buffer = pool->copy(&sn, sizeof(sn)),
    });

    fk_app_UdpMessage message = fk_app_UdpMessage_init_default;
    message.deviceId.funcs.encode = pb_encode_data;
    message.deviceId.arg = device_id_data;
    message.status = status;

    auto encoded = pool_->encode(fk_app_UdpMessage_fields, &message);
    if (encoded == nullptr) {
        loginfo("encode failed");
        return false;
    }

    loginfo("publishing");

    if (!udp_.beginPacket(IPAddress(224, 1, 2, 3), NetworkUdpDiscoveryPort)) {
        logerror("begin failed!");
        return false;
    }

    udp_.write(encoded->buffer, encoded->size);

    if (udp_.endPacket() != 0) {
        logerror("send failed!");
        return false;
    }

    return true;
}

} // namespace fk

#endif
