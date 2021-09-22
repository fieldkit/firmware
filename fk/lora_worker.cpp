#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "hal/hal.h"
#include "lora_manager.h"
#include "lora_packetizer.h"
#include "platform.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

struct OutgoingPackets {
    EncodedMessage *packets;
};

static OutgoingPackets packetize(Pool &pool) {
    auto gs = get_global_state_ro();
    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(gs.get(), pool);
    if (!packets) {
        return OutgoingPackets{ nullptr };
    }
    return OutgoingPackets{ *packets };
}

static void update_activity(Pool &pool) {
    auto gs = get_global_state_rw();
    gs.get()->lora.activity = fk_uptime();
}

void LoraWorker::run(Pool &pool) {
    auto lock = lora_mutex.acquire(0);
    if (!lock) {
        loginfo("already running");
        return;
    }

    auto outgoing = packetize(pool);
    if (outgoing.packets == nullptr) {
        loginfo("no packets");
        return;
    }

    LoraManager lora{ get_lora_network() };
    if (!lora.begin(pool)) {
        return;
    }

    auto iterator = outgoing.packets;
    while (iterator != nullptr) {
        // We do this in each pass of the loop because one of them may fail. We
        // don't bother transmitting, though.
        if (!lora.join_if_necessary(pool)) {
            return;
        }

        // We defer all error handling to the manager, just try and send all the
        // packets we've got. It's hard to rationalize retry behavior when
        // you've got more than one flow.
        if (!lora.send_bytes(LoraDataPort, iterator->buffer, iterator->size, pool)) {
            break;
        }
        iterator = iterator->link;
        if (iterator != nullptr) {
            loginfo("lora packet delay (%" PRIu32 ")", LoraPacketDelay);
            fk_delay(LoraPacketDelay);
        }
    }

    lora.stop();

    update_activity(pool);
}

} // namespace fk
