#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "lora_manager.h"
#include "lora_packetizer.h"
#include "platform.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

tl::expected<EncodedMessage *, Error> packetize(Pool &pool) {
    auto gs = get_global_state_ro();

    LoraPacketizer packetizer;
    return packetizer.packetize(gs.get(), pool);
}

void LoraWorker::run(Pool &pool) {
    LoraManager lora{ get_lora_network() };

    auto expected_packets = packetize(pool);
    if (!expected_packets || *expected_packets == nullptr) {
        loginfo("no packets");
        return;
    }

    if (!lora.begin()) {
        return;
    }

    auto confirmed = true;

    auto packets = *expected_packets;
    auto tries = 0u;
    while (packets != nullptr && tries < LoraSendTries) {
        if (!lora.configure_tx(5, 1)) {
            logerror("configuring tx");
            return;
        }

        switch (lora.send_bytes(LoraDataPort, packets->buffer, packets->size, confirmed, pool)) {
        case LoraErrorCode::None: {
            // Next packet!
            packets = packets->link;
            tries = 0;
            confirmed = false;

            if (packets != nullptr) {
                loginfo("lora packet delay (%" PRIu32 ")", LoraPacketDelay);
                fk_delay(LoraPacketDelay);
            }

            break;
        }
        case LoraErrorCode::DataLength: {
            tries++;

            loginfo("lora packet delay (%" PRIu32 ")", LoraPacketDelay);
            fk_delay(LoraPacketDelay);

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

} // namespace fk
