#include "lora_worker.h"

#include "hal/metal/metal_lora.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static bool joined = false;
static bool asleep = false;
static int32_t counter = 0;

void LoraWorker::run(Pool &pool) {
    auto lora = get_lora_network();

    if (!joined) {
        if (!lora->begin()) {
            return;
        }

        const char *app_eui = "0000000000000000";
        const char *app_key = "39e98dbaa08feed53d5f68d43d0ef981";

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
