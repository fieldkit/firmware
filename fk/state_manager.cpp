#include "state_manager.h"
#include "state.h"
#include "device_name.h"
#include "state_ref.h"
#include "hal/random.h"

namespace fk {

FK_DECLARE_LOGGER("gsm");

bool GlobalStateManager::initialize(Pool &pool) {
    auto gs = get_global_state_rw();

    auto name = fk_device_name_generate(pool);

    strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));

    FK_ASSERT(fk_random_fill_u8(gs.get()->general.generation, sizeof(gs.get()->general.generation)) == GenerationLength);

    return true;
}

bool GlobalStateManager::rebuild() {
    return true;
}

}
