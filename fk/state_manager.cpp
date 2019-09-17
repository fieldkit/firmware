#include "state_manager.h"
#include "state.h"
#include "device_name.h"
#include "state_ref.h"
#include "hal/random.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("gsm");

bool GlobalStateManager::initialize(Pool &pool) {
    auto gs = get_global_state_rw();

    auto name = fk_device_name_generate(pool);

    strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));

    FK_ASSERT(fk_random_fill_u8(gs.get()->general.generation, sizeof(gs.get()->general.generation)) == GenerationLength);

    char gen_string[GenerationLength * 2 + 1];
    bytes_to_hex_string(gen_string, sizeof(gen_string), gs.get()->general.generation, sizeof(gs.get()->general.generation));
    loginfo("(generated) name: '%s'", gs.get()->general.name);
    loginfo("(generated) gen: %s", gen_string);

    return true;
}

bool GlobalStateManager::rebuild() {
    return true;
}

}
