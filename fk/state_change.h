#pragma once

#include <os.h>

#include "state.h"

namespace fk {

class StateChange {
public:
    StateChange() { }
    virtual ~StateChange() { }

public:
    virtual void apply(GlobalState *gs) = 0;
    virtual const char *source() const = 0;

};

template<typename T>
class SimpleStateChange : public StateChange {
private:
    const char *source_;
    T fn_;

public:
    SimpleStateChange(const char *source, T fn) : source_(source), fn_(fn) {
    }

public:
    void apply(GlobalState *gs) override {
        fn_(gs);
    }

    const char *source() const override {
        return source_;
    }

};

template<typename T>
SimpleStateChange<T> *change_state(T fn) {
    auto task = os_task_name();
    return new SimpleStateChange<T>(task, fn);
}

}
