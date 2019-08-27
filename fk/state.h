#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"
#include "pool.h"
#include "containers.h"

namespace fk {

struct SensorDetails {
    const char *name;
    const char *uom;
    bool has_live_vaue;
    float live_value;
};

struct ModuleDetails {
    const char *name;
    SensorDetails *sensors;
    size_t nsensors;
};

using SensorStateCollection = std::list<SensorDetails, pool_allocator<SensorDetails>>;

struct ModuleState {
public:
    uint16_t position;
    uint16_t manufacturer;
    uint16_t kind;
    uint16_t version;
    char name[MaximumNameLength];
    SensorStateCollection sensors;

public:
    ModuleState(Pool &pool);
    ModuleState(Pool &pool, ModuleHeader header);

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

using ModuleStateCollection = std::list<ModuleState, pool_allocator<ModuleState>>;

struct GlobalState {
private:
    MallocPool pool_{ "gs:mods", 1024 };

public:
    GeneralState general;
    RuntimeState runtime;
    PowerState power;
    PeripheralState peripheral;
    GpsState gps;
    NetworkState network;
    ModuleStateCollection modules;

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

class GlobalStateManager {
public:
    bool initialize();
    bool rebuild();

};

}
