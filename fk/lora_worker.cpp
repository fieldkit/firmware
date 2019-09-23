#include "lora_worker.h"

#include "hal/metal/metal_lora.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static bool joined = false;
static bool asleep = false;
static int32_t counter = 0;

void LoraWorker::run(Pool &pool) {
    auto lora = get_lora_network();

    if (!joined) {
        auto gs = get_global_state_ro();
        if (!gs.get()->lora.configured) {
            return;
        }

        if (!lora->begin()) {
            return;
        }

        auto app_key = bytes_to_hex_string_pool(gs.get()->lora.app_key, LoraAppKeyLength, pool);
        auto app_eui = bytes_to_hex_string_pool(gs.get()->lora.app_eui, LoraAppEuiLength, pool);

        if (!lora->join(app_eui, app_key)) {
            return;
        }

        joined = true;
    }
    else {
        if (asleep) {
            if (!lora->wake()) {
                logerror("error waking");
                return;
            }

            asleep = false;
        }
        else {
            if (!lora->begin()) {
                return;
            }
        }
    }

    for (uint8_t i = 0; i < 10; ++i) {
        if (!lora->send_bytes((uint8_t *)&counter, sizeof(counter))) {
            logerror("tx failed");
        }

        counter++;

        fk_delay(1000);
    }

    if (!lora->sleep(OneHourMs)) {
        logerror("error sleeping");
        return;
    }

    asleep = true;

    loginfo("done");
}

}
