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
    wcl_.stop();
    return true;
}

bool MetalWifi::begin(WifiSettings settings) {
    SPI1.begin();

    wifi_.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (wifi_.status() == WL_NO_SHIELD) {
        fkb_external_println("fk: no wifi");
        return false;
    }

    if (settings.create) {
        wifi_.beginAP(settings.ssid, settings.password);
    }
    else {
        wifi_.begin(settings.ssid, settings.password);
    }

    settings_ = settings;

    return true;
}

bool MetalWifi::serve() {
    server_.begin();

    IPAddress ip = WiFi.localIP();

    fkb_external_println("fk: connected (ip = %d.%d.%d.%d)", ip[0], ip[1], ip[2], ip[3]);

    if (!mdns_.begin(ip, settings_.name)) {
        fkb_external_println("fk: unable to start mdns responder!");
        return false;
    }

    // TODO: Build a real name using settings._name
    mdns_.addServiceRecord("station._fk", 80, MDNSServiceTCP);

    return true;
}

WifiStatus MetalWifi::status() {
    switch (wifi_.status()) {
    case WL_NO_SHIELD: return WifiStatus::Error;
    case WL_CONNECTED: return WifiStatus::Connected;
    case WL_AP_LISTENING: return WifiStatus::Listening;
    }
    return WifiStatus::Ready;
}

uint32_t MetalWifi::ip_address() {
    return wifi_.localIP();
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
    wifi_.end();

    return true;
}

}

#endif
