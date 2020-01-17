#include "state.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("gs");

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
    for (auto &status : physical_modules) {
        status.meta = nullptr;
        status.header = { };
        status.status = ModuleStatus::Empty;
    }

    for (auto &m : modules) {
        if (m.found.physical()) {
            auto bay = m.found.position;

            FK_ASSERT(bay < MaximumNumberOfPhysicalModules);

            auto &status = physical_modules[bay];
            status.header = m.found.header;
            status.meta = m.meta;
            status.status = m.status;
        }
    }
}

void GlobalState::released(uint32_t locked) const {
    auto elapsed = fk_uptime() - locked;
    if (elapsed > 100) {
        logwarn("read (%" PRIu32 "ms)", elapsed);
    }
}

void GlobalState::released(uint32_t locked) {
    loginfo("modified (%" PRIu32 "ms)", fk_uptime() - locked);
    version++;
}

}
