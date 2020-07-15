#include "state.h"
#include "state_ref.h"
#include "storage/meta_ops.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

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

void GlobalState::update_data_stream(File const &file) {
    storage.data.size = file.size();
    storage.data.block = file.record();
    readings.number = file.record();
}

void GlobalState::update_meta_stream(File const &file) {
    storage.meta.size = file.size();
    storage.meta.block = file.record();
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
    logverbose("modified (%" PRIu32 "ms)", fk_uptime() - locked);
    version++;
}

bool GlobalState::flush(Pool &pool) const {
    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    if (!storage.begin()) {
        return false;
    }

    MetaOps ops{ storage };
    if (!ops.write_state(this, pool)) {
        return false;
    }

    return true;
}

GpsState *GpsState::clone(Pool &pool) const {
    auto clone = new (pool) GpsState();
    memcpy(clone, this, sizeof(GpsState));
    return clone;
}

GpsState const *GlobalState::location(Pool &pool) const {
    return gps.clone(pool);
}

}
