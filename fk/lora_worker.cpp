#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "hal/hal.h"
#include "lora_manager.h"
#include "lora_packetizer.h"
#include "platform.h"
#include "state_ref.h"
#include "state_manager.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

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
    case LoraWorkOperation::Location: {
        location(lora, pool);
        break;
    }
    }

    lora.stop();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->lora.activity = fk_uptime();
        gs->lora.sessions++;
    });
}

bool LoraWorker::factory_reset(LoraManager &lora, Pool &pool) {
    loginfo("factory-reset");

    // Factory reset the module.
    if (!lora.factory_reset()) {
        return false;
    }

    // NOTE We can power down the module.

    return true;
}

bool LoraWorker::configure(LoraManager &lora, Pool &pool) {
    loginfo("configure");

    // This is basically an order to try and join a network.
    if (!lora.join_if_necessary(pool)) {
        return false;
    }

    return true;
}

bool LoraWorker::packets(LoraManager &lora, uint8_t port, EncodedMessage *iterator, Pool &pool) {
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
        if (!lora.send_bytes(port, iterator->buffer, iterator->size, pool)) {
            return lora.power_cycle();
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

bool LoraWorker::readings(LoraManager &lora, Pool &pool) {
    loginfo("readings");

    return packets<LoraReadingsPacketizer>(lora, LoraDataPort, pool);
}

bool LoraWorker::location(LoraManager &lora, Pool &pool) {
    loginfo("location");

    return packets<LoraLocationPacketizer>(lora, LoraLocationPort, pool);
}

} // namespace fk
