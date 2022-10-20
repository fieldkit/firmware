#include "hal/metal/esp32_network.h"

namespace fk {

#if defined(__SAMD51__) && defined(FK_NETWORK_ESP32)

FK_DECLARE_LOGGER("network");

Esp32Network::Esp32Network() {
}

bool Esp32Network::begin(NetworkSettings settings, Pool *pool) {
    return MetalNetwork::begin(settings, pool);
}

bool Esp32Network::start_ap(NetworkSettings settings) {
    WiFi.beginAP(settings.ssid);
    return true;
}

PoolPointer<NetworkConnection> *Esp32Network::open_connection(const char *scheme, const char *hostname, uint16_t port) {
    WiFiClient wcl;

    if (strcmp(scheme, "https") == 0) {
        logdebug("connection %s %d (ssl) rssi=%d", hostname, port, WiFi.RSSI());
        if (!wcl.connectSSL(hostname, port)) {
            return nullptr;
        }
    } else {
        logdebug("connection %s %d rssi=%d", hostname, port, WiFi.RSSI());
        if (!wcl.connect(hostname, port)) {
            return nullptr;
        }
    }

    return create_network_connection_wrapper<MetalNetworkConnection>(wcl);
}

#endif

} // namespace fk