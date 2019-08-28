#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"
#include "pool.h"
#include "containers.h"

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
    uint32_t flags;
    SensorState *sensors;
    size_t nsensors;
};

struct ModulesState {
    Pool *pool;
    ModuleState *modules;
    size_t nmodules;

    ModulesState(Pool *pool) : pool(pool) {
    }
};

struct RuntimeState {
    uint32_t activity{ 0 };
};

struct PowerState {
    uint32_t charge{ 0 };
    uint32_t voltage{ 0 };
};

struct NetworkState {
    uint32_t enabled{ 0 };
    uint32_t ip{ 0 };
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
};

struct ProgressState {
    const char *message;
    float progress;
};

struct DisplayState {
    ProgressState progress;
    const char *message;
};

struct GlobalState {
public:
    GeneralState general;
    RuntimeState runtime;
    PowerState power;
    PeripheralState peripheral;
    GpsState gps;
    NetworkState network;
    DisplayState display;
    ModulesState *modules;

public:
    GlobalState();

};

}
