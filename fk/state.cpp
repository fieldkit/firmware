#include "state.h"

namespace fk {

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

}
