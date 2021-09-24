#pragma once

#include "common.h"
#include "config.h"

namespace fk {

struct WifiNetworkInfo {
    bool valid{ false };
    bool create{ false };
    char ssid[WifiMaximumSsidLength]{};
    char password[WifiMaximumPasswordLength]{};

    WifiNetworkInfo() {
    }

    WifiNetworkInfo(WifiNetworkInfo const &other) {
        valid = other.valid;
        create = other.create;
        strncpy(this->ssid, other.ssid, WifiMaximumSsidLength);
        strncpy(this->password, other.password, WifiMaximumPasswordLength);
    }

    explicit WifiNetworkInfo(bool valid, bool create) : valid(valid), create(create) {
    }

    explicit WifiNetworkInfo(const char *ssid) : valid(true), create(true) {
        strncpy(this->ssid, ssid, WifiMaximumSsidLength);
        this->password[0] = 0;
    }

    explicit WifiNetworkInfo(const char *ssid, const char *password) {
        strncpy(this->ssid, ssid, WifiMaximumSsidLength);
        strncpy(this->password, password, WifiMaximumPasswordLength);
    }

    WifiNetworkInfo &operator=(const WifiNetworkInfo &other) {
        valid = other.valid;
        create = other.create;
        strncpy(this->ssid, other.ssid, WifiMaximumSsidLength);
        strncpy(this->password, other.password, WifiMaximumPasswordLength);
        return *this;
    }
};

struct NetworkConfiguration {
    WifiNetworkInfo wifi_networks[WifiMaximumNumberOfNetworks];
    WifiNetworkInfo selected;
    uint32_t modified;
};

struct NetworkState {
    uint32_t enabled{ 0 };
    uint32_t connected{ 0 };
    uint32_t ip{ 0 };
    uint32_t bytes_tx{ 0 };
    uint32_t bytes_rx{ 0 };
    uint32_t activity{ 0 };
    char ssid[WifiMaximumSsidLength];
};

struct MainNetworkState {
    NetworkState state;
    NetworkConfiguration config;
};

struct DebuggingUdpTraffic {
    uint32_t start_time{ 0 };
    uint32_t stop_time{ 0 };
    uint32_t quantity{ 0 };
    uint32_t interval{ 0 };
    uint32_t duration{ 1000 };
    bool readings_triggered{ false };

    DebuggingUdpTraffic() {
    }

    DebuggingUdpTraffic(uint32_t stop_time, uint32_t quantity, uint32_t interval, bool readings_triggered)
        : stop_time(stop_time), quantity(quantity), interval(interval), readings_triggered(readings_triggered) {
    }
};

struct TransmissionState {
    bool enabled{ false };
    char url[HttpMaximumUrlLength]{};
    char token[HttpMaximumTokenLength]{};
    uint32_t data_cursor{ 0 };
    uint32_t meta_cursor{ 0 };
};

} // namespace fk
