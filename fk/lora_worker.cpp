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

LoraWorker::LoraWorker() : work_{ LoraWorkOperation::Readings } {
}

LoraWorker::LoraWorker(LoraWork work) : work_(work) {
}

void LoraWorker::run(Pool &pool) {
    auto lock = lora_mutex.acquire(0);
    if (!lock) {
        loginfo("already running");
        return;
    }

    LoraManager lora{ get_lora_network() };
    if (!lora.begin(pool)) {
        return;
    }

    switch (work_.op) {
    case LoraWorkOperation::FactoryReset: {
        factory_reset(lora, pool);
        break;
    }
    case LoraWorkOperation::Configure: {
        configure(lora, pool);
        break;
    }
    case LoraWorkOperation::Readings: {
        readings(lora, pool);
        break;
    }
    }

    lora.stop();

    update_activity(pool);
}

bool LoraWorker::factory_reset(LoraManager &lora, Pool &pool) {
    // Factory reset the module.
    if (!lora.factory_reset()) {
        return false;
    }

    // NOTE We can power down the module.

    return true;
}

bool LoraWorker::configure(LoraManager &lora, Pool &pool) {
    // This is basically an order to try and join a network.
    if (!lora.join_if_necessary(pool)) {
        return false;
    }

    return true;
}

bool LoraWorker::readings(LoraManager &lora, Pool &pool) {
    auto outgoing = packetize(pool);
    auto iterator = outgoing.packets;

    // Log no packets to begin with, just in case that's a surprise.
    if (iterator == nullptr) {
        loginfo("no packets");
        return true;
    }

    // Loop over linked list of packets to send.
    while (iterator != nullptr) {
        // We do this in each pass of the loop because one of them may fail. We
        // don't bother transmitting, though.
        if (!lora.join_if_necessary(pool)) {
            return false;
        }

        // We defer all error handling to the manager, just try and send all the
        // packets we've got. It's hard to rationalize retry behavior when
        // you've got more than one flow.
        if (!lora.send_bytes(LoraDataPort, iterator->buffer, iterator->size, pool)) {
            return false;
        }

        // We only delay if there's more to send.
        iterator = iterator->link;
        if (iterator != nullptr) {
            loginfo("lora packet delay (%" PRIu32 ")", LoraPacketDelay);
            fk_delay(LoraPacketDelay);
        }
    }

    return true;
}

} // namespace fk
