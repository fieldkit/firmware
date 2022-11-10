#include "hal/metal/winc1500_network.h"

namespace fk {

#if defined(__SAMD51__) && !defined(FK_NETWORK_ESP32)

FK_DECLARE_LOGGER("network");

class StaticWiFiCallbacks : public WiFiCallbacks {
private:
    constexpr static size_t ExpectedWiFiBufferSize = 1472;
    constexpr static size_t NumberOfBuffers = 3;

    struct Buffer {
        bool taken{ false };
        void *ptr{ nullptr };
    };
    Buffer buffers_[NumberOfBuffers];

public:
    void initialize(Pool &pool);
    void *malloc(size_t size) override;
    void free(void *ptr) override;
    bool busy(uint32_t elapsed) override;
};

static StaticWiFiCallbacks staticWiFiCallbacks;

Winc1500Network::Winc1500Network() {
}

bool Winc1500Network::begin(NetworkSettings settings, Pool *pool) {
    staticWiFiCallbacks.initialize(*pool);

    WiFiSocketClass::callbacks = &staticWiFiCallbacks;

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    /**
     * Very important that this IRQ be handled immediately or terrible
     * things begin to happen to the network transfers because this
     * causes contention/leaks in the buffer memory of the module.
     */
    NVIC_SetPriority(EIC_11_IRQn, OS_IRQ_PRIORITY_SYSTICK - 1);

    return MetalNetwork::begin(settings, pool);
}

bool Winc1500Network::connected() {
    uint8_t mac_address[6];
    WiFi.APClientMacAddress(mac_address);
    loginfo("remote mac: %s", bytes_to_hex_string_pool(mac_address, sizeof(mac_address), network_pool()));
    return true;
}

bool Winc1500Network::start_ap(NetworkSettings settings) {
    IPAddress ip{ 192, 168, 2, 1 };
    WiFi.beginAP(settings.ssid, 2, ip);
    return true;
}

PoolPointer<NetworkConnection> *Winc1500Network::open_connection(const char *scheme, const char *hostname, uint16_t port) {
    WiFiClient wcl;

    if (strcmp(scheme, "https") == 0) {
        logdebug("connection %s %d (ssl) rssi=%d", hostname, port, WiFi.RSSI());
        if (!wcl.connectSSL(hostname, port, true)) {
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

void Winc1500Network::disable() {
    digitalWrite(WINC1500_POWER, LOW);
    SPI1.end();

    pinMode(WINC1500_CS, INPUT_PULLUP);
    pinMode(WINC1500_IRQ, INPUT_PULLUP);
    pinMode(WINC1500_RESET, INPUT_PULLUP);
}

void Winc1500Network::enable() {
    pinMode(WINC1500_CS, OUTPUT);
    pinMode(WINC1500_IRQ, INPUT);
    pinMode(WINC1500_RESET, OUTPUT);

    digitalWrite(WINC1500_POWER, HIGH);
    SPI1.begin();
}

void StaticWiFiCallbacks::initialize(Pool &pool) {
    for (auto i = 0u; i < NumberOfBuffers; ++i) {
        buffers_[i].ptr = pool.malloc(ExpectedWiFiBufferSize);
        buffers_[i].taken = false;
    }
}

void *StaticWiFiCallbacks::malloc(size_t size) {
    FK_ASSERT(size == ExpectedWiFiBufferSize);

    for (auto i = 0u; i < NumberOfBuffers; ++i) {
        if (!buffers_[i].taken) {
            buffers_[i].taken = true;
            alogf(LogLevels::DEBUG, "network", "malloc() = 0x%p", buffers_[i].ptr);
            return buffers_[i].ptr;
        }
    }

    return nullptr;
}

void StaticWiFiCallbacks::free(void *ptr) {
    for (auto i = 0u; i < NumberOfBuffers; ++i) {
        if (buffers_[i].ptr == ptr) {
            buffers_[i].taken = false;
            alogf(LogLevels::DEBUG, "network", "free(0x%p)", buffers_[i].ptr);
            return;
        }
    }

    alogf(LogLevels::WARN, "network", "free of old network buffer, ignoring 0x%p", ptr);
}

bool StaticWiFiCallbacks::busy(uint32_t elapsed) {
    return true;
}

#endif

} // namespace fk