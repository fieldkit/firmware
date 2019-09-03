#include "lora_worker.h"

#include "hal/metal/metal_lora.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static bool joined = false;

void LoraWorker::run(Pool &pool) {
    auto lora = get_lora_network();

    if (!joined) {
        if (!lora->begin()) {
            return;
        }

        joined = true;
    }

    for (uint8_t i = 0; i < 10; ++i) {
        uint8_t data[] = { 0xaa, i };
        if (!lora->send_bytes(data, sizeof(data))) {
            logerror("tx failed");
        }

        fk_delay(1000);
    }

    loginfo("done");
}

}
