#include <tiny_printf.h>

#include "hal/metal/metal_network.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("network");

MetalNetworkConnection::MetalNetworkConnection() {
}

MetalNetworkConnection::MetalNetworkConnection(WiFiClient wcl) : wcl_(wcl) {
}

MetalNetworkConnection::~MetalNetworkConnection() {
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
    return wcl_.read(buffer, size);
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
    auto buffers = reinterpret_cast<buffered_write_t*>(arg);
    buffers->buffer[buffers->position++] = c;
    if (buffers->position == buffers->buffer_size) {
        buffers->flush();
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
    wcl_.flush();
    wcl_.stop();
    return true;
}

bool MetalNetwork::begin(NetworkSettings settings) {
    board.enable_wifi();

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        return false;
    }

    if (settings.ssid != nullptr) {
        if (settings.create) {
            loginfo("creating '%s'", settings.ssid);
            if (settings.password != nullptr) {
                WiFi.beginAP(settings.ssid, settings.password);
            }
            else {
                WiFi.beginAP(settings.ssid);
            }
        }
        else {
            loginfo("connecting '%s'", settings.ssid);
            WiFi.begin(settings.ssid, settings.password);
        }
    }

    settings_ = settings;
    enabled_ = true;

    return true;
}

bool MetalNetwork::serve() {
    server_.begin();

    IPAddress ip = WiFi.localIP();

    strncpy(service_name_, settings_.name, sizeof(service_name_) - 5);
    auto n = std::min(strlen(service_name_), sizeof(service_name_) - 5);
    strncpy(service_name_ + n, "._fk", 4);
    service_name_[n + 4] = 0;

    if (!mdns_.begin(ip, settings_.name)) {
        logerror("unable to start mdns responder!");
        return false;
    }

    mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP);

    loginfo("ready (ip = %d.%d.%d.%d) (service = %s)",
           ip[0], ip[1], ip[2], ip[3], service_name_);

    if (!settings_.create) {
        ntp_.start();
    }

    return true;
}

NetworkStatus MetalNetwork::status() {
    switch (WiFi.status()) {
    case WL_NO_SHIELD: return NetworkStatus::Error;
    case WL_CONNECTED: return NetworkStatus::Connected;
    case WL_AP_LISTENING: return NetworkStatus::Listening;
    }
    return NetworkStatus::Ready;
}

uint32_t MetalNetwork::ip_address() {
    return WiFi.localIP();
}

NetworkConnection *MetalNetwork::accept() {
    mdns_.run();

    if (!settings_.create) {
        ntp_.service();
    }

    auto wcl = server_.available(nullptr, true);
    if (!wcl) {
        return nullptr;
    }

    // TODO: MALLOC
    return new MetalNetworkConnection(wcl);
}

bool MetalNetwork::stop() {
    mdns_.removeServiceRecord(80, MDNSServiceTCP);
    // Ensure the previous removal gets loose.
    fk_delay(100);
    udp_.stop();
    ntp_.stop();
    WiFi.end();
    enabled_ = false;
    board.disable_wifi();
    return true;
}

bool MetalNetwork::enabled() {
    return enabled_;
}

}

#endif
