#pragma once

#include "common.h"
#include "config.h"
#include "pool.h"
#include "containers.h"
#include "modules/module_factory.h"
#include "modules/shared/uuid.h"

namespace fk {

struct SensorState {
    const char *name;
    const char *unitOfMeasure;
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
};

using ModuleReadingsCollection = std::list<ModuleMetaAndReadings , pool_allocator<ModuleMetaAndReadings>>;

struct TakenReadings {
    uint32_t time;
    uint32_t number;
    ModuleReadingsCollection readings;

    TakenReadings() {
    }

    TakenReadings(uint32_t time, uint32_t number, ModuleReadingsCollection readings) : time(time), number(number), readings(readings) {
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
        return { readings_time, readings_number, readings };
    }
};

struct RuntimeState {
    uint32_t activity{ 0 };
    uint32_t readings{ 0 };
};

struct PowerState {
    uint32_t charge{ 0 };
    uint32_t voltage{ 0 };
};

struct WifiNetworkInfo {
    bool valid;
    bool create;
    char ssid[WifiMaximumSsidLength];
    char password[WifiMaximumPasswordLength];
    uint32_t modified;

    WifiNetworkInfo() {
    }

    WifiNetworkInfo(bool valid, bool create) : valid(valid), create(create) {
    }

    WifiNetworkInfo(const char *ssid, const char *password) {
        strncpy(this->ssid, ssid, WifiMaximumSsidLength);
        strncpy(this->password, password, WifiMaximumPasswordLength);
    }
};

struct NetworkConfiguration {
    WifiNetworkInfo wifi_networks[MaximumNumberOfWifiNetworks];
    WifiNetworkInfo selected;
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
    uint32_t size;
    uint32_t block;
    uint32_t modified;
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
};

struct GlobalState {
public:
    GeneralState general;
    RuntimeState runtime;
    PowerState power;
    PeripheralState peripheral;
    GpsState gps;
    MainNetworkState network;
    ProgressState progress;
    StorageState storage;
    LoraState lora;
    ModulesState *modules;

public:
    GlobalState();

};

}
