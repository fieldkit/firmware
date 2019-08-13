#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"

namespace fk {

struct SensorDetails {
    const char *name;
    const char *uom;
};

struct ModuleDetails {
    const char *name;
    SensorDetails *sensors;
    size_t nsensors;
};

struct ModuleState {
public:
    uint16_t position;
    uint16_t manufacturer;
    uint16_t kind;
    uint16_t version;

public:
    ModuleState();
    ModuleState(ModuleHeader header);

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

struct GlobalState {
public:
    RuntimeState runtime;
    PowerState power;
    PeripheralState peripheral;
    GpsState gps;
    NetworkState network;
    ModuleState modules[MaximumNumberOfModules];

public:
    GlobalState();

};

template<typename T>
class GlobalStateRef {
private:
    T value_;

public:
    GlobalStateRef(T value) : value_(value) {
    }
    virtual ~GlobalStateRef() {
    }

public:
    T get() {
        return value_;
    }

};

GlobalStateRef<GlobalState const*> get_global_state_ro();

GlobalStateRef<GlobalState*> get_global_state_rw();

}
