#include "state.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("gsm");

static GlobalState gs;

GlobalStateRef<const GlobalState*> get_global_state_ro() {
    return { &gs };
}

GlobalStateRef<GlobalState*> get_global_state_rw() {
    return { &gs };
}

ModuleState::ModuleState(Pool &pool) : sensors(pool) {
}

ModuleState::ModuleState(Pool &pool, ModuleHeader header) : sensors(pool) {
    manufacturer = header.manufacturer;
    kind = header.kind;
    version = header.version;
}

GlobalState::GlobalState() : modules(pool_) {
}

bool GlobalStateManager::initialize() {
    StaticPool<MaximumNameLength> pool{ "gsm" };

    auto gs = get_global_state_rw();

    auto name = fk_device_name_generate(pool);
    loginfo("name = '%s'", name);

    strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));

    return true;
}

bool GlobalStateManager::rebuild() {
    return true;
}
}
