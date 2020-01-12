#pragma once

#include "state.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

namespace fk {

template<typename T>
class GlobalStateRef {
private:
    RwLock::Lock lock_;
    bool readonly_;
    T value_;

public:
    GlobalStateRef(RwLock::Lock lock, bool readonly, T value) : lock_(std::move(lock)), readonly_(readonly), value_(value) {
    }

    GlobalStateRef(GlobalStateRef &&ref) : lock_(std::move(ref.lock_)), readonly_(ref.readonly_), value_(std::move(ref.value_)) {
    }

    virtual ~GlobalStateRef() {
        if (!readonly_) {
            value_->released();
        }
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
