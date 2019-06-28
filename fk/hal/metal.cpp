#include "metal.h"

#if defined(ARDUINO)

namespace fk {

MetalWifiConnection::MetalWifiConnection() {
}

MetalWifiConnection::MetalWifiConnection(WiFiClient wcl) : wcl_(wcl) {
}

WifiConnectionStatus MetalWifiConnection::status() {
    if (wcl_.connected()) {
        return WifiConnectionStatus::Connected;
    }
    return WifiConnectionStatus::Disconnected;
}

bool MetalWifiConnection::available() {
    return wcl_.available();
}

int32_t MetalWifiConnection::read(uint8_t *buffer, size_t size) {
    return wcl_.read(buffer, size);
}

int32_t MetalWifiConnection::write(const char *str) {
    return wcl_.write(str, strlen(str));
}

int32_t MetalWifiConnection::write(uint8_t *buffer, size_t size) {
    return wcl_.write(buffer, size);
}

bool MetalWifiConnection::stop() {
    wcl_.flush();
    wcl_.stop();
    return true;
}

bool MetalWifi::begin(WifiSettings settings) {
    SPI1.begin();

    fkb_external_println("fk: checking wifi...");

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        fkb_external_println("fk: no wifi");
        return false;
    }

    fkb_external_println("fk: connecting/creating AP...");

    if (settings.create) {
        WiFi.beginAP(settings.ssid, settings.password);
    }
    else {
        WiFi.begin(settings.ssid, settings.password);
    }

    settings_ = settings;

    return true;
}

bool MetalWifi::serve() {
    server_.begin();

    IPAddress ip = WiFi.localIP();

    strncpy(service_name_, settings_.name, sizeof(service_name_) - 5);
    auto n = std::min(strlen(service_name_), sizeof(service_name_) - 5);
    strncpy(service_name_ + n, "._fk", 4);
    service_name_[n + 4] = 0;

    if (!mdns_.begin(ip, settings_.name)) {
        fkb_external_println("fk: unable to start mdns responder!");
        return false;
    }

    mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP);

    fkb_external_println("fk: ready (ip = %d.%d.%d.%d) (service = %s)",
                         ip[0], ip[1], ip[2], ip[3], service_name_);

    return true;
}

WifiStatus MetalWifi::status() {
    switch (WiFi.status()) {
    case WL_NO_SHIELD: return WifiStatus::Error;
    case WL_CONNECTED: return WifiStatus::Connected;
    case WL_AP_LISTENING: return WifiStatus::Listening;
    }
    return WifiStatus::Ready;
}

uint32_t MetalWifi::ip_address() {
    return WiFi.localIP();
}

WifiConnection *MetalWifi::accept() {
    mdns_.run();

    auto wcl = server_.available();
    if (!wcl) {
        return nullptr;
    }

    // TODO: Temporary
    return new MetalWifiConnection(wcl);
}

bool MetalWifi::stop() {
    WiFi.end();

    return true;
}

}

#endif
