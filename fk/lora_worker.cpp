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
    auto tries = 0;

    if (!lora.begin()) {
        return;
    }

    while (packets != nullptr && tries < 3) {
        switch (lora.send_bytes(LoraDataPort, packets->buffer, packets->size)) {
        case LoraErrorCode::None: {
            // Next packet!
            packets = packets->link;
            tries = 0;
            break;
        }
        case LoraErrorCode::NotJoined: {
            tries++;
            // Try joining and then we'll transmit again.
            if (!lora.join_if_necessary(pool)) {
                // Force the loop to end.
                packets = nullptr;
            }
            break;
        }
        default: {
            // Force the loop to end.
            packets = nullptr;
            break;
        }
        }
    }

    lora.stop();
}

}
