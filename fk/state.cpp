#include "state.h"
#include "state_ref.h"

namespace fk {

static GlobalState gs;

GlobalStateRef<const GlobalState*> get_global_state_ro() {
    auto lock = data_lock.acquire_read(UINT32_MAX);
    FK_ASSERT(lock);
    return { std::move(lock), true, &gs };
}

GlobalStateRef<GlobalState*> get_global_state_rw() {
    auto lock = data_lock.acquire_write(UINT32_MAX);
    FK_ASSERT(lock);
    return { std::move(lock), false, &gs };
}

GlobalStateRef<GlobalState const*> try_get_global_state_ro() {
    auto lock = data_lock.acquire_read(0);
    if (!lock) {
        return { std::move(lock), false, nullptr };
    }
    return { std::move(lock), true, &gs };
}

GlobalState::GlobalState() : version(0) {
}

void GlobalState::update_physical_modules(ConstructedModulesCollection const &modules) {
    for (auto &m : modules) {
        if (m.found.physical()) {
            auto bay = m.found.position;

            FK_ASSERT(bay < MaximumNumberOfPhysicalModules);

            auto &status = physical_modules[bay];
            status.available = true;
            status.configured = m.meta != nullptr;
            status.initialized = m.initialized;
            status.header = m.found.header;
            status.meta = m.meta;
        }
    }
}

void GlobalState::released() const {
}

void GlobalState::released() {
    version++;
}

}
