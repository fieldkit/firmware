#include "lora_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "state_ref.h"
#include "utilities.h"
#include "platform.h"
#include "lora_packetizer.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

static uint32_t joined = 0;
static uint32_t asleep = 0;

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
    auto lora = get_lora_network();
    auto tries = 3;

    auto expected_packets = packetize(pool);
    if (!expected_packets || *expected_packets == nullptr) {
        loginfo("no packets");
        return;
    }

    for (auto p = *expected_packets; p != nullptr; p = p->link) {
        fk_dump_memory("LORA ", p->buffer, p->size);
    }

    auto packets = *expected_packets;

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
        }

        if (!lora->send_bytes(packets->buffer, packets->size)) {
            switch (lora->error()) {
            case LoraErrorCode::NotJoined: {
                logerror("rejoining");
                joined = 0;
                tries--;
                break;
            }
            case LoraErrorCode::Mac: {
                logerror("retrying");
                tries--;
                break;
            }
            case LoraErrorCode::DataLength: {
                fk_delay(1000);
                logerror("retrying");
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
            packets = packets->link;
            if (packets == nullptr) {
                tries = 0;
            }
            else {
                tries = 3;
            }
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
