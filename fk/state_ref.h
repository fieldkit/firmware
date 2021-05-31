#pragma once

#include "state.h"

#include "hal/mutex.h"

namespace fk {

template<typename T>
class GlobalStateRef {
private:
    uint32_t created_;
    Lock lock_;
    bool readonly_;
    T value_;

public:
    GlobalStateRef(Lock lock, bool readonly, T value) : created_(fk_uptime()), lock_(std::move(lock)), readonly_(readonly), value_(value) {
    }

    GlobalStateRef(GlobalStateRef &&ref) : created_(ref.created_), lock_(std::move(ref.lock_)), readonly_(ref.readonly_), value_(std::move(ref.value_)) {
    }

    virtual ~GlobalStateRef() {
        value_->released(created_);
    }

public:
    T get() {
        return value_;
    }

    operator bool() {
        return value_ != nullptr;
    }

};

GlobalStateRef<GlobalState const*> get_global_state_ro();

GlobalStateRef<GlobalState const*> try_get_global_state_ro();

GlobalStateRef<GlobalState*> get_global_state_rw();

}
