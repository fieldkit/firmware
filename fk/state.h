#pragma once

#include "common.h"
#include "config.h"
#include "modules/modules.h"

namespace fk {

struct ModuleState {
public:
    uint16_t manufacturer;
    uint16_t kind;
    uint16_t version;

public:
    ModuleState();
    ModuleState(ModuleHeader header);

};

struct RuntimeState {
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
    uint32_t enabled{ 0 };
    uint32_t fixed{ 0 };
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

GlobalStateRef<const GlobalState*> get_global_state_ro();

GlobalStateRef<GlobalState*> get_global_state_rw();

class StateChange {
public:
    StateChange() { }
    virtual ~StateChange() { }

public:
    virtual void apply(GlobalState *gs) = 0;

};

template<typename T>
class SimpleStateChange : public StateChange {
private:
    T fn_;

public:
    SimpleStateChange(T fn) : fn_(fn) { }

public:
    void apply(GlobalState *gs) override {
        fn_(gs);
    }

};

template<typename T>
SimpleStateChange<T> *change_state(T fn) {
    return new SimpleStateChange<T>(fn);
}

}
