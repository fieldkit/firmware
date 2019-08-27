#include "state_manager.h"
#include "state.h"
#include "device_name.h"

namespace fk {

FK_DECLARE_LOGGER("gsm");

bool GlobalStateManager::initialize(Pool &pool) {
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
