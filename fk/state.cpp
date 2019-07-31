#include "state.h"

namespace fk {

static GlobalState gs;

GlobalStateRef<const GlobalState*> get_global_state_ro() {
    return { &gs };
}

GlobalStateRef<GlobalState*> get_global_state_rw() {
    return { &gs };
}

ModuleState::ModuleState() {
}

ModuleState::ModuleState(ModuleHeader header) {
    manufacturer = header.manufacturer;
    kind = header.kind;
    version = header.version;
}

GlobalState::GlobalState() {
}

}
