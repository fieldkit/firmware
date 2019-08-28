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
    T value_;

public:
    GlobalStateRef(RwLock::Lock lock, T value) : lock_(std::move(lock)), value_(value) {
    }
    GlobalStateRef(GlobalStateRef &&ref) : lock_(std::move(ref.lock_)), value_(std::move(ref.value_)) {
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
