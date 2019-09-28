#pragma once

#include "common.h"
#include "config.h"
#include "pool.h"
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

struct ModulesState {
    Pool *pool;
    ModuleState *modules;
    size_t nmodules;
    uint32_t readings_time;

    ModulesState(Pool *pool) : pool(pool) {
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
    char ssid[64];
    char password[64];
};

struct NetworkConfiguration {
    WifiNetworkInfo wifi_networks[MaximumNumberOfWifiNetworks];
    WifiNetworkInfo selected;
};

struct NetworkState {
    uint32_t enabled{ 0 };
    uint32_t connected{ 0 };
    uint32_t ip{ 0 };
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
    bool recording;
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
