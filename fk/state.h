#pragma once

#include <lwcron/lwcron.h>

#include "common.h"
#include "config.h"
#include "pool.h"
#include "collections.h"
#include "modules/module_factory.h"
#include "modules/shared/uuid.h"
#include "platform.h"

namespace fk {

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
    SensorState *sensors;
    size_t nsensors;
};

struct ModuleMetaAndReadings {
    uint16_t position;
    fk_uuid_t *id;
    ModuleMetadata const *meta;
    ModuleSensors const *sensors;
    ModuleReadings const *readings;
    ModuleConfiguration configuration;
};

using ModuleReadingsCollection = collection<ModuleMetaAndReadings>;

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
    Pool *pool;
    ModuleState *modules;
    size_t nmodules;
    uint32_t readings_time;
    uint32_t readings_number;
    ModuleReadingsCollection readings;

    ModulesState(Pool *pool) : pool(pool), readings{ pool } {
    }

    TakenReadings taken() {
        return { readings_time, readings_number, ModuleReadingsCollection(readings) };
    }
};

struct RuntimeState {
    uint32_t startup_time{ 0 };
    uint32_t activity{ 0 };
    uint32_t readings{ 0 };
};

struct PowerState {
    uint32_t charge{ 0 };
    uint32_t voltage{ 0 };
    float vbus{ 0 };
    float vs{ 0 };
    float ma{ 0 };
    float mw{ 0 };
};

struct WifiNetworkInfo {
    bool valid;
    bool create;
    char ssid[WifiMaximumSsidLength];
    char password[WifiMaximumPasswordLength];

    WifiNetworkInfo() {
    }

    WifiNetworkInfo(WifiNetworkInfo const &other) {
        valid = other.valid;
        create = other.create;
        strncpy(this->ssid, other.ssid, WifiMaximumSsidLength);
        strncpy(this->password, other.password, WifiMaximumPasswordLength);
    }

    WifiNetworkInfo(bool valid, bool create) : valid(valid), create(create) {
    }

    WifiNetworkInfo(const char *ssid) : valid(true), create(true) {
        strncpy(this->ssid, ssid, WifiMaximumSsidLength);
        this->password[0] = 0;
    }

    WifiNetworkInfo(const char *ssid, const char *password) {
        strncpy(this->ssid, ssid, WifiMaximumSsidLength);
        strncpy(this->password, password, WifiMaximumPasswordLength);
    }
};

struct NetworkConfiguration {
    WifiNetworkInfo wifi_networks[MaximumNumberOfWifiNetworks];
    WifiNetworkInfo selected;
    uint32_t modified;
};

struct NetworkState {
    uint32_t enabled{ 0 };
    uint32_t connected{ 0 };
    uint32_t ip{ 0 };
    uint32_t bytes_tx{ 0 };
    uint32_t bytes_rx{ 0 };
    char ssid[WifiMaximumSsidLength];
};

struct MainNetworkState {
    NetworkState state;
    NetworkConfiguration config;
};

struct GpsState {
    uint8_t enabled{ 0 };
    uint8_t fix{ 0 };
    uint8_t satellites{ 0 };
    uint64_t time{ 0 };
    uint16_t hdop{ 0 };
    float longitude { 0.0f };
    float latitude { 0.0f };
    float altitude { 0.0f };
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

    NotificationState() {
    }

    NotificationState(const char *message) : created(fk_uptime()), message(message), delay(0) {
    }
};

struct Schedule {
    lwcron::CronSpec cron;
    uint32_t interval = 0;
    uint32_t repeated = 0;
    uint32_t duration = 0;

    Schedule& operator= (const fk_app_Schedule &s) {
        interval = s.interval;
        repeated = s.repeated;
        duration = s.duration;
        cron = lwcron::CronSpec::interval(s.interval);
        return *this;
    }
};

struct SchedulerState {
    Schedule readings;
    Schedule network;
    Schedule gps;
    Schedule lora;
};

struct ReadingsState {
    uint32_t number;
    uint32_t time;
};

struct GlobalState {
public:
    uint32_t version;
    GeneralState general;
    RuntimeState runtime;
    PowerState power;
    PeripheralState peripheral;
    GpsState gps;
    MainNetworkState network;
    NotificationState notification;
    ProgressState progress;
    StorageState storage;
    LoraState lora;
    SchedulerState scheduler;
    PhysicalModuleState physical_modules[MaximumNumberOfPhysicalModules];
    SdCardState sd_card;
    // TODO Merge these.
    ModulesState *modules;
    ReadingsState readings;

public:
    GlobalState();

public:
    void update_physical_modules(ConstructedModulesCollection const &modules);
    void released(uint32_t locked) const;
    void released(uint32_t locked);

};

}
