#pragma once

#include <fk-app-protocol.h>
#include <lwcron/lwcron.h>

#include "collections.h"
#include "common.h"
#include "config.h"
#include "platform.h"
#include "pool.h"
#include "protobuf.h"

#include "hal/battery_gauge.h" // For MeterReading
#include "modules/bridge/data.h"
#include "state/dynamic.h"

namespace fk {

class File;

struct ScheduledTime {
    uint32_t now;
    uint32_t time;
    uint32_t seconds;
};

enum class BatteryStatus { Unknown, Good, External, Low, Dangerous };

inline bool battery_status_is_low_power(BatteryStatus status) {
    return status == BatteryStatus::Dangerous || status == BatteryStatus::Low;
}

inline const char *battery_status_to_string(BatteryStatus status) {
    switch (status) {
    case BatteryStatus::Unknown:
        return "unknown";
    case BatteryStatus::Good:
        return "good";
    case BatteryStatus::External:
        return "external";
    case BatteryStatus::Low:
        return "low";
    case BatteryStatus::Dangerous:
        return "dangerous";
    default:
        return "unknown";
    }
}

struct RuntimeState {
    uint32_t startup_time{ 0 };
    uint32_t readings{ 0 };
};

struct PowerState {
    bool low_battery{ false };
    MeterReading battery{};
    MeterReading solar{};
    BatteryStatus battery_status{ BatteryStatus::Unknown };
    bool allow_deep_sleep{ false };
    float charge{ 0 };
};

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

struct GpsState {
public:
    uint8_t enabled{ 0 };
    uint8_t fix{ 0 };
    uint8_t satellites{ 0 };
    uint64_t time{ 0 };
    uint16_t hdop{ 0 };
    float longitude{ 0.0f };
    float latitude{ 0.0f };
    float altitude{ 0.0f };
    uint32_t chars{ 0 };

public:
    GpsState *clone(Pool &pool) const;
};

struct GeneralState {
    char name[MaximumNameLength];
    uint8_t generation[GenerationLength];
    uint32_t recording{ 0 };
};

struct ProgressState {
    const char *operation;
    float progress;
};

struct StreamState {
    uint32_t size{ 0 };
    uint32_t block{ 0 };
    uint32_t modified{ 0 };
};

struct MemoryState {
    uint32_t installed{ 0 };
    uint32_t used{ 0 };
};

struct StorageState {
    MemoryState spi;
    MemoryState qspi;
    StreamState data;
    StreamState meta;

    bool is_phylum() const {
        return data.block > 0 && meta.block == 0;
    }
};

struct StorageStreamUpdate {
    uint32_t size;
    uint32_t records;
};

struct StorageUpdate {
    StorageStreamUpdate meta;
    StorageStreamUpdate data;
    uint32_t nreadings;
    uint32_t installed;
    uint32_t used;
};

struct LoraState {
    bool configured;
    uint8_t device_eui[LoraDeviceEuiLength];
    uint8_t app_key[LoraAppKeyLength];
    uint8_t app_eui[LoraAppEuiLength];
    uint8_t device_address[LoraDeviceAddressLength];
    uint8_t network_session_key[LoraNetworkSessionKeyLength];
    uint8_t app_session_key[LoraAppSessionKeyLength];
    uint32_t uplink_counter;
    uint32_t downlink_counter;
    bool has_module;
    uint32_t joined;
    uint32_t asleep;
    uint32_t join_failures;
    uint32_t tx_successes;
    uint32_t tx_failures;
};

struct SdCardState {
    bool found{ false };
    bool working{ false };
    uint32_t size{ 0 };
    uint32_t remaining{ 0 };
    uint32_t written{ 0 };
};

struct NotificationState {
    uint32_t created{ 0 };
    const char *message{ nullptr };
    uint32_t delay{ 0 };

    explicit NotificationState() {
    }

    explicit NotificationState(const char *message) : created(fk_uptime()), message(message), delay(0) {
    }
};

struct OpenMenu {
    uint32_t time;
    bool readings;
};

struct DisplayState {
    OpenMenu open_menu;
};

struct Interval {
    uint32_t start;
    uint32_t end;
    uint32_t interval{ 0 };
};

struct Schedule {
    lwcron::CronSpec cron{};
    uint32_t interval{ 0 };
    uint32_t repeated{ 0 };
    uint32_t duration{ 0 };
    uint32_t jitter{ 0 };
    Interval intervals[MaximumScheduleIntervals];
    ScheduledTime upcoming;

    Schedule &operator=(const fk_app_Schedule &s);

    void recreate();
    void simple(uint32_t interval);
};

struct SchedulerState {
    Schedule readings{};
    Schedule network{};
    Schedule gps{};
    Schedule lora{};
    Schedule backup{};
};

struct ReadingsState {
    uint32_t nreadings{ 0 };
    uint32_t time{ 0 };
};

struct TransmissionState {
    bool enabled{ false };
    char url[HttpMaximumUrlLength]{};
    char token[HttpMaximumTokenLength]{};
    uint32_t data_cursor{ 0 };
    uint32_t meta_cursor{ 0 };
};

struct UpcomingUpdate {
    ScheduledTime readings;
    ScheduledTime network;
    ScheduledTime gps;
    ScheduledTime lora;
    ScheduledTime backup;
};

struct DebuggingUdpTraffic {
    uint32_t stop_time{ 0 };
    uint32_t quantity{ 0 };
    uint32_t interval{ 0 };
    bool readings_triggered{ false };

    DebuggingUdpTraffic() {
    }

    DebuggingUdpTraffic(uint32_t stop_time, uint32_t quantity, uint32_t interval, bool readings_triggered)
        : stop_time(stop_time), quantity(quantity), interval(interval), readings_triggered(readings_triggered) {
    }
};

struct DebuggingState {
    uint32_t ec_excite_delay{ 10 };
    DebuggingUdpTraffic udp_traffic;
};

struct GlobalState {
public:
    uint32_t version{ 0 };
    state::DynamicState dynamic;
    GeneralState general{};
    RuntimeState runtime{};
    PowerState power{};
    GpsState gps{};
    MainNetworkState network{};
    NotificationState notification{};
    DisplayState display{};
    ProgressState progress{};
    StorageState storage{};
    LoraState lora{};
    SchedulerState scheduler{};
    SdCardState sd_card{};
    ReadingsState readings{};
    TransmissionState transmission{};
    DebuggingState debugging{};

public:
    GlobalState();

public:
    void apply(StorageUpdate &update);
    void apply(UpcomingUpdate &update);
    void released(uint32_t locked) const;
    void released(uint32_t locked);
    bool flush(Pool &pool);

public:
    GpsState const *location(Pool &pool) const;
};

} // namespace fk
