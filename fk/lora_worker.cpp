#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "lora_manager.h"
#include "lora_packetizer.h"
#include "state_ref.h"
#include "utilities.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

tl::expected<EncodedMessage*, Error> packetize(Pool &pool) {
    auto gs = get_global_state_ro();
    if (gs.get()->modules == nullptr) {
        return nullptr;
    }

    auto taken = gs.get()->modules->taken();
    if (taken.time == 0) {
        return nullptr;
    }

    LoraPacketizer packetizer;
    return packetizer.packetize(taken, pool);
}

void LoraWorker::run(Pool &pool) {
    LoraManager lora{ get_lora_network() };

    auto expected_packets = packetize(pool);
    if (!expected_packets || *expected_packets == nullptr) {
        loginfo("no packets");
        return;
    }

    auto packets = *expected_packets;

    while (packets != nullptr) {
        if (!lora.join_if_necessary(pool)) {
            break;
        }

        if (!lora.send_bytes(LoraDataPort, packets->buffer, packets->size)) {
            break;
        }
        else {
            packets = packets->link;
        }
    }

    lora.stop();
}

}
