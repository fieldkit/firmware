#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "state_ref.h"
#include "utilities.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static uint32_t joined = 0;
static uint32_t asleep = 0;

void LoraWorker::run(Pool &pool) {
    auto lora = get_lora_network();
    auto tries = 3;

    while (tries > 0) {
        if (joined == 0 || joined > fk_uptime() || fk_uptime() - joined > OneDayMs) {
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

        uint32_t counter = fk_random_i32(0, UINT32_MAX);
        if (!lora->send_bytes((uint8_t *)&counter, sizeof(counter))) {
            switch (lora->error()) {
            case LoraErrorCode::NotJoined: {
                logerror("rejoining");
                joined = 0;
                tries--;
                break;
            }
            case LoraErrorCode::Mac: {
                logerror("retring");
                tries--;
                break;
            }
            default: {
                logerror("tx failed");
                tries = 0;
                break;
            }
            }
        }
        else {
            tries = 0;
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
