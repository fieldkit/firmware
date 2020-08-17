#pragma once

#include <lwcron/lwcron.h>
#include <fk-app-protocol.h>

#include "common.h"
#include "config.h"
#include "pool.h"
#include "collections.h"
#include "platform.h"
#include "protobuf.h"

#include "modules/bridge/data.h"
#include "hal/battery_gauge.h" // For MeterReading

namespace fk {

class File;

struct SensorState {
    const char *name;
    const char *unit_of_measure;
    uint32_t flags;
    bool has_live_vaue;
    float live_value;
};

struct ModuleState {
public:
    uint32_t position;
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t version;
    const char *name;
    const char *display_name_key;
    fk_uuid_t *id;
    uint32_t flags;
    EncodedMessage *status_message;
    SensorState *sensors;
    size_t nsensors;
};

struct TakenReadings {
    uint32_t time;
    uint32_t number;
    ConstructedModulesCollection constructed_modules;
    ModuleReadingsCollection readings;

    TakenReadings() {
    }

    TakenReadings(uint32_t time, uint32_t number, ModuleReadingsCollection readings) :
        time(time), number(number), readings(std::move(readings)) {
    }

    TakenReadings(uint32_t time, uint32_t number, ConstructedModulesCollection constructed_modules, ModuleReadingsCollection readings) :
        time(time), number(number), constructed_modules(std::move(constructed_modules)), readings(std::move(readings)) {
    }
};

struct ModulesState {
    Pool *pool{ nullptr };
    ModuleState *modules{ nullptr };
    size_t nmodules{ 0 };
    uint32_t readings_time{ 0 };
    uint32_t readings_number{ 0 };
    ModuleReadingsCollection readings{ };

    explicit ModulesState(Pool *pool) : pool(pool), readings{ pool } {
    }

    TakenReadings taken() {
        return { readings_time, readings_number, ModuleReadingsCollection(readings) };
    }
};

struct RuntimeState {
    uint32_t startup_time{ 0 };
    uint32_t readings{ 0 };
    bool power_save{ false };
};

struct PowerState {
    float charge{ 0 };
    MeterReading battery{ };
    MeterReading solar{ };
};

struct WifiNetworkInfo {
    bool valid{ false };
    bool create{ false };
    char ssid[WifiMaximumSsidLength]{ };
    char password[WifiMaximumPasswordLength]{ };

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

    WifiNetworkInfo& operator=(const WifiNetworkInfo &other) {
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
    float longitude { 0.0f };
    float latitude { 0.0f };
    float altitude { 0.0f };
    uint32_t chars{ 0 };

public:
    GpsState *clone(Pool &pool) const;
};

struct PeripheralState {
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

struct StorageState {
    StreamState data;
    StreamState meta;
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

struct PhysicalModuleState {
    ModuleStatus status{ ModuleStatus::Unknown };
    ModuleHeader header;
    ModuleMetadata const *meta;
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

struct Interval {
    uint32_t start;
    uint32_t end;
    uint32_t interval{ 0 };
};

struct Schedule {
    lwcron::CronSpec cron{ };
    uint32_t interval{ 0 };
    uint32_t repeated{ 0 };
    uint32_t duration{ 0 };
    uint32_t jitter{ 0 };
    Interval intervals[MaximumScheduleIntervals];

    Schedule& operator=(const fk_app_Schedule &s);

    void recreate();
};

struct SchedulerState {
    Schedule readings{ };
    Schedule network{ };
    Schedule gps{ };
    Schedule lora{ };
};

struct ReadingsState {
    uint32_t number{ 0 };
    uint32_t time{ 0 };
};

struct TransmissionState {
    bool enabled{ false };
    char url[HttpMaximumUrlLength]{ };
    char token[HttpMaximumTokenLength]{ };
    uint32_t data_cursor{ 0 };
    uint32_t meta_cursor{ 0 };
};

struct GlobalState {
public:
    uint32_t version{ 0 };
    GeneralState general{ };
    RuntimeState runtime{ };
    PowerState power{ };
    PeripheralState peripheral{ };
    GpsState gps{ };
    MainNetworkState network{ };
    NotificationState notification{ };
    ProgressState progress{ };
    StorageState storage{ };
    LoraState lora{ };
    SchedulerState scheduler{ };
    PhysicalModuleState physical_modules[MaximumNumberOfPhysicalModules]{ };
    SdCardState sd_card{ };
    // TODO Merge these.
    ModulesState *modules{ };
    ReadingsState readings{ };
    TransmissionState transmission{ };

public:
    GlobalState();

public:
    void update_physical_modules(ConstructedModulesCollection const &modules);
    void update_data_stream(File const &file);
    void update_meta_stream(File const &file);
    void released(uint32_t locked) const;
    void released(uint32_t locked);
    bool flush(Pool &pool);

public:
    GpsState const *location(Pool &pool) const;

};

}
