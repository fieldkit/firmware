#include "lora_worker.h"

#include "hal/metal/metal_lora.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static uint32_t joined = 0;
static uint32_t asleep = 0;

void LoraWorker::run(Pool &pool) {
    auto lora = get_lora_network();

    if (joined == 0) {
        auto gs = get_global_state_ro();
        if (!gs.get()->lora.configured) {
            loginfo("no configuration");
            return;
        }

        if (!lora->begin()) {
            logwarn("missing module");
            return;
        }

        auto app_key = bytes_to_hex_string_pool(gs.get()->lora.app_key, LoraAppKeyLength, pool);
        auto app_eui = bytes_to_hex_string_pool(gs.get()->lora.app_eui, LoraAppEuiLength, pool);

        if (!lora->join(app_eui, app_key)) {
            return;
        }

        joined = fk_uptime();
    }
    else {
        if (asleep > 0) {
            if (!lora->wake()) {
                logerror("error waking");
                return;
            }

            asleep = 0;
        }
        else {
            if (!lora->begin()) {
                return;
            }
        }
    }

    if (!lora->sleep(OneHourMs)) {
        logerror("error sleeping");
        return;
    }

    asleep = fk_uptime();

    loginfo("done");
}

}
