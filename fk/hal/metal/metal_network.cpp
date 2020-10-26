#include <tiny_printf.h>

#include "utilities.h"
#include "hal/metal/metal_network.h"

#if defined(__SAMD51__)

#include <WiFiSocket.h>

namespace fk {

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
    void initialize(Pool &pool) {
        for (auto i = 0u; i < NumberOfBuffers; ++i) {
            buffers_[i].ptr = pool.malloc(ExpectedWiFiBufferSize);
            buffers_[i].taken = false;
        }
    }

    void *malloc(size_t size) override {
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

    void free(void *ptr) override {
        for (auto i = 0u; i < NumberOfBuffers; ++i) {
            if (buffers_[i].ptr == ptr) {
                buffers_[i].taken = false;
                alogf(LogLevels::DEBUG, "network", "free(0x%p)", buffers_[i].ptr);
                return;
            }
        }

        alogf(LogLevels::WARN, "network", "free of old network buffer, ignoring 0x%p", ptr);
    }

    bool busy(uint32_t elapsed) override {
        return true;
    }

};

static StaticWiFiCallbacks staticWiFiCallbacks;

const char *get_wifi_status(uint8_t status) {
    switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
    case WL_AP_LISTENING: return "WL_AP_LISTENING";
    case WL_AP_CONNECTED: return "WL_AP_CONNECTED";
    case WL_AP_FAILED: return "WL_AP_FAILED";
    case WL_PROVISIONING: return "WL_PROVISIONING";
    case WL_PROVISIONING_FAILED: return "WL_PROVISIONING_FAILED";
    default: return "Unknown";
    }
}

const char *get_wifi_status() {
    return get_wifi_status(WiFi.status());
}

FK_DECLARE_LOGGER("network");

MetalNetworkConnection::MetalNetworkConnection() {
}

MetalNetworkConnection::MetalNetworkConnection(WiFiClient wcl) : wcl_(wcl) {
    if (debugging_) {
        size_ = StandardPageSize;
        buffer_ = reinterpret_cast<uint8_t *>(fk_standard_page_malloc(size_, __func__));
        bzero(buffer_, size_);
    }
}

MetalNetworkConnection::~MetalNetworkConnection() {
    if (buffer_ != nullptr) {
        fk_standard_page_free(buffer_);
    }
}

NetworkConnectionStatus MetalNetworkConnection::status() {
    if (wcl_.connected()) {
        return NetworkConnectionStatus::Connected;
    }
    return NetworkConnectionStatus::Disconnected;
}

bool MetalNetworkConnection::available() {
    return wcl_.available();
}

int32_t MetalNetworkConnection::read(uint8_t *buffer, size_t size) {
    auto nread = wcl_.read(buffer, size);
    if (nread < 0) {
        return 0;
    }

    if (buffer_ != nullptr) {
        auto copying = std::min<size_t>(size_ - position_, nread);
        if (copying > 0) {
            logdebug("[%d] copying %zd + %zd", wcl_.socket(), position_, copying);
            memcpy(buffer_ + position_, buffer, copying);
            position_ += copying;
        }
    }

    return nread;
}

int32_t MetalNetworkConnection::write(const char *str) {
    return wcl_.write(str, strlen(str));
}

int32_t MetalNetworkConnection::write(const uint8_t *buffer, size_t size) {
    return wcl_.write(buffer, size);
}

int32_t MetalNetworkConnection::writef(const char *str, ...) {
    va_list args;
    va_start(args, str);
    auto rv = vwritef(str, args);
    va_end(args);
    return rv;
}

typedef struct buffered_write_t {
    uint8_t buffer[256];
    size_t buffer_size{ 256 };
    size_t position{ 0 };
    WiFiClient *wcl;
    int32_t return_value{ 0 };
    int32_t flush() {
        if (position > 0) {
            auto wrote = wcl->write(buffer, position);
            if (wrote > 0) {
                return_value += wrote;
            }
            position = 0;
        }
        return return_value;
    }
} buffered_write_t;

static void write_connection(char c, void *arg) {
    if (c > 0) {
        auto buffers = reinterpret_cast<buffered_write_t*>(arg);
        buffers->buffer[buffers->position++] = c;
        if (buffers->position == buffers->buffer_size) {
            buffers->flush();
        }
    }
}

int32_t MetalNetworkConnection::vwritef(const char *str, va_list args) {
    buffered_write_t buffers;
    buffers.wcl = &wcl_;
    tiny_vfctprintf(write_connection, &buffers, str, args);
    buffers.flush();
    return buffers.return_value;
}

int32_t MetalNetworkConnection::socket() {
    return wcl_.socket();
}

uint32_t MetalNetworkConnection::remote_address() {
    return wcl_.remoteIP();
}

bool MetalNetworkConnection::stop() {
    auto s = wcl_.socket();

    wcl_.flush();
    wcl_.stop();

    if (buffer_ != nullptr && position_ > 0) {
        char temp[32];
        tiny_snprintf(temp, sizeof(temp), "CDGB-%d ", s);
        fk_dump_memory(temp, buffer_, position_);
    }

    return true;
}

bool MetalNetwork::begin(NetworkSettings settings, Pool *pool) {
    if (availability_ == Availability::Unavailable) {
        logwarn("wifi unavailable");
        return false;
    }

    FK_ASSERT(pool != nullptr);

    pool_ = pool;

    staticWiFiCallbacks.initialize(*pool_);

    WiFiSocketClass::callbacks = &staticWiFiCallbacks;

    get_board()->enable_wifi();

    fk_delay(100);

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        get_board()->disable_wifi();
        availability_ = Availability::Unavailable;
        return false;
    }

    availability_ = Availability::Available;

    if (settings.ssid != nullptr) {
        if (settings.create) {
            if (settings.password != nullptr && settings.password[0] != 0) {
                loginfo("creating '%s' '%s'", settings.ssid, settings.password);
                WiFi.beginAP(settings.ssid, settings.password);
            }
            else {
                loginfo("creating '%s'", settings.ssid);
                IPAddress ip{ 192, 168, 2, 1 };
                WiFi.beginAP(settings.ssid, 2, ip);
            }
        }
        else {
            loginfo("connecting '%s'", settings.ssid);
            WiFi.begin(settings.ssid, settings.password);
        }
    }

    settings_ = settings;
    enabled_ = true;
    serving_ = false;

    return true;
}

bool MetalNetwork::serve() {
    serving_ = true;

    mdns_discovery_.pool(pool_);
    if (!mdns_discovery_.start()) {
        logwarn("mdns discovery failed");
    }

    udp_discovery_.pool(pool_);
    if (!udp_discovery_.start()) {
        logwarn("udp discovery failed");
    }

    synchronize_time();

    IPAddress ip = WiFi.localIP();
    loginfo("ready (ip = %d.%d.%d.%d) (status = %s)",
            ip[0], ip[1], ip[2], ip[3], get_wifi_status());

    return true;
}

NetworkStatus MetalNetwork::status() {
    switch (WiFi.status()) {
    case WL_NO_SHIELD: return NetworkStatus::Error;
    case WL_CONNECTED: return NetworkStatus::Connected;
    case WL_AP_LISTENING: return NetworkStatus::Listening;
    case WL_AP_CONNECTED: return NetworkStatus::Connected;
    }
    return NetworkStatus::Ready;
}

uint32_t MetalNetwork::ip_address() {
    return WiFi.localIP();
}

PoolPointer<NetworkListener> *MetalNetwork::listen(uint16_t port) {
    auto listener = create_weak_network_listener_wrapper<MetalNetworkListener>(*pool_, port);

    if (!listener->get<MetalNetworkListener>()->begin()) {
        return nullptr;
    }

    return listener;
}

void MetalNetwork::service(Pool *pool) {
    mdns_discovery_.service(pool);
    udp_discovery_.service(pool);
    ntp_.service();
}

PoolPointer<NetworkConnection> *MetalNetwork::open_connection(const char *scheme, const char *hostname, uint16_t port) {
    WiFiClient wcl;

    auto https = "https";
    if (strcmp(scheme, https) == 0) {
        logdebug("connection %s %d (ssl)", hostname, port);
        if (!wcl.connectSSL(hostname, port, true)) {
            return nullptr;
        }
    }
    else {
        logdebug("connection %s %d", hostname, port);
        if (!wcl.connect(hostname, port)) {
            return nullptr;
        }
    }

    return create_network_connection_wrapper<MetalNetworkConnection>(wcl);
}

bool MetalNetwork::stop() {
    if (enabled_) {
        if (serving_) {
            ntp_.stop();
            // Ensure the previous removal gets loose?
            fk_delay(500);
            udp_discovery_.stop();
            mdns_discovery_.stop();
            serving_ = false;
        }
        WiFi.end();
        enabled_ = false;
        get_board()->disable_wifi();
    }
    pool_ = nullptr;
    return true;
}

bool MetalNetwork::enabled() {
    return enabled_;
}

bool MetalNetwork::synchronize_time() {
    if (!settings_.create) {
        ntp_.pool(pool_);
        ntp_.start();
    }

    return true;
}

bool MetalNetwork::get_mac_address(uint8_t *address) {
    WiFi.macAddress(address);
    return true;
}

const char *MetalNetwork::get_ssid() {
    return WiFi.SSID();
}

bool MetalNetwork::get_created_ap() {
    auto status = WiFi.status();
    return status == WL_AP_LISTENING ||  status == WL_AP_CONNECTED;
}

NetworkScan MetalNetwork::scan(Pool &pool) {
    size_t number_ssids = WiFi.scanNetworks();


    auto ssids = (const char **)pool.malloc(sizeof(const char *) * number_ssids);

    for (auto i = 0u; i < number_ssids; ++i) {
        auto ssid = WiFi.SSID(i);
        if (ssid != nullptr) {
            ssids[i] = pool.strdup(ssid);
        }
    }

    return NetworkScan{ ssids, number_ssids };
}

MetalNetworkListener::MetalNetworkListener(uint16_t port) : port_(port) {
}

bool MetalNetworkListener::begin() {
    server_.begin();

    return true;
}

PoolPointer<NetworkConnection> *MetalNetworkListener::accept() {
    auto wcl = server_.available(nullptr, true);
    if (!wcl) {
        return nullptr;
    }

    return create_network_connection_wrapper<MetalNetworkConnection>(wcl);
}

bool MetalNetworkListener::stop() {
    return true;
}

}

#endif
