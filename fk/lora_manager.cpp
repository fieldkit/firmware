#include "lora_manager.h"
#include "state_ref.h"
#include "state_manager.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

LoraManager::LoraManager(LoraNetwork *network) : network_(network) {
}

bool LoraManager::available() {
    auto gs = get_global_state_ro();
    return gs.get()->lora.configured;
}

static LoraState get_lora_state() {
    auto gs = get_global_state_ro();
    return gs.get()->lora;
}

bool LoraManager::join_if_necessary(Pool &pool) {
    GlobalStateManager gsm;

    auto state = get_lora_state();

    if (state.joined == 0 || state.joined > fk_uptime() || fk_uptime() - state.joined > OneDayMs) {
        if (!state.configured) {
            loginfo("no configuration");
            return false;
        }

        if (!network_->begin()) {
            logwarn("missing module");
            gsm.apply([=](GlobalState *gs) {
                gs->lora.has_module = false;
                gs->lora.joined = 0;
                gs->lora.asleep = 0;
            });
            return false;
        }
        else {
            gsm.apply([=](GlobalState *gs) {
                gs->lora.has_module = true;
                gs->lora.joined = 0;
                gs->lora.asleep = 0;
            });
        }

        auto app_key = bytes_to_hex_string_pool(state.app_key, LoraAppKeyLength, pool);
        auto app_eui = bytes_to_hex_string_pool(state.app_eui, LoraAppEuiLength, pool);

        auto joined = network_->join(app_eui, app_key);

        gsm.apply([=](GlobalState *gs) {
            gs->lora.has_module = true;
            gs->lora.joined = joined ? fk_uptime() : 0;
            gs->lora.asleep = 0;
        });

        awake_ = true;
    }

    if (state.asleep > 0) {
        loginfo("waking");

        if (!network_->wake()) {
            logerror("error waking");
            return false;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->lora.asleep = fk_uptime();
        });

        awake_ = true;
    }

    return true;
}

bool LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size) {
    auto success = network_->send_bytes(port, data, size);

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        if (network_->error() == LoraErrorCode::NotJoined) {
            gs->lora.joined = 0;
        }
        if (success) {
            gs->lora.tx_successes++;
        }
        else {
            gs->lora.tx_failures++;
        }
    });

    return success;
}

void LoraManager::stop() {
    if (awake_) {
        if (!network_->sleep(OneHourMs)) {
            logerror("error sleeping");
        }

        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
            gs->lora.asleep = fk_uptime();
        });

        awake_ = false;
    }
}

} // namespace fk
