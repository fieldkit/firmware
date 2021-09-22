#include "lora_manager.h"
#include "state_manager.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

LoraManager::LoraManager(LoraNetwork *network) : network_(network) {
}

static LoraState get_lora_global_state() {
    auto gs = get_global_state_ro();
    return gs.get()->lora;
}

bool LoraManager::begin(Pool &pool) {
    GlobalStateManager gsm;

    auto success = network_->begin();
    auto state = get_lora_global_state();
    if (state.asleep > 0) {
        loginfo("waking");

        if (!network_->wake()) {
            logerror("error waking");
            return false;
        }
    }

    awake_ = true;

    gsm.apply([=](GlobalState *gs) {
        if (gs->lora.has_module != success) {
            loginfo("has-module: %d", success);
            gs->lora.has_module = success;
            gs->lora.joined = 0;
            gs->lora.asleep = 0;
            /*
            if (success) {
                auto device_eui = network_->device_eui();
                memcpy(gs->lora.device_eui, device_eui, LoraDeviceEuiLength);
                loginfo("(loaded) lora device eui: %s", bytes_to_hex_string_pool(device_eui, LoraDeviceEuiLength, pool));
            }
            */
        }
    });

    if (success) {
        // Just doing this to log errors during startup.
        verify_status(pool);
    }

    return success;
}

bool LoraManager::factory_reset() {
    return network_->factory_reset();
}

bool LoraManager::verify_status(Pool &pool) {
    auto state = get_lora_global_state();

    if (state.joined > 0) {
        loginfo("already joined");
        return true;
    }

    if (is_null_byte_array(state.device_eui, LoraDeviceEuiLength)) {
        logerror("module missing device-eui");
        fk_dump_memory("device-eui: ", state.device_eui, LoraDeviceEuiLength);
        return false;
    }

    if (is_null_byte_array(state.app_key, LoraAppKeyLength)) {
        logerror("module missing app-key");
        fk_dump_memory("app-key: ", state.app_key, LoraAppKeyLength);
        return false;
    }

    return true;
}

bool LoraManager::join_if_necessary(Pool &pool) {
    if (!verify_status(pool)) {
        return false;
    }

    auto joined = false;
    auto state = get_lora_global_state();
    auto module_state = network_->get_state(pool);
#if defined(FK_LORA_TESTING_FORCE_JOIN)
    auto force_join = state.joined == 0;
#else
    auto force_join = false;
#endif
    if (force_join || is_null_byte_array(module_state->device_address, LoraDeviceAddressLength)) {
        if (force_join) {
            loginfo("force-join enabled");
        } else {
            loginfo("module missing devaddr, joining via otaa");
        }

        LoraOtaaJoin otaa;
        otaa.device_eui = bytes_to_hex_string_pool(state.device_eui, LoraDeviceEuiLength, pool);
        otaa.join_eui = bytes_to_hex_string_pool(state.join_eui, LoraJoinEuiLength, pool);
        otaa.app_key = bytes_to_hex_string_pool(state.app_key, LoraAppKeyLength, pool);

        joined = network_->join(otaa);
    } else {
        loginfo("joining via stored abp");

        joined = network_->join_resume();

        network_->get_state(pool);
    }

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->lora.has_module = true;
        gs->lora.joined = joined ? fk_uptime() : 0;
        gs->lora.activity = fk_uptime();
        gs->lora.asleep = 0;
    });

    awake_ = true;

    return joined;
}

bool LoraManager::configure_tx(uint8_t power_index, uint8_t data_rate) {
    return network_->configure_tx(power_index, data_rate);
}

static bool get_should_confirm() {
    auto lora = get_lora_global_state();
    if (lora.tx_successes > 0 && LoraConfirmEvery > 0) {
        if ((lora.tx_successes % LoraConfirmEvery) == 0) {
            return true;
        }
    }
    if (lora.confirmed_tries > 0) {
        if (lora.confirmed_tries == 3) {
            logwarn("%d confirmed tries failed", lora.confirmed_tries);
            lora.confirmed_tries = 0;
            return false;
        }
        return true;
    }
    return false;
}

LoraErrorCode LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed, Pool &pool) {
    auto should_confirm = get_should_confirm();
    auto success = network_->send_bytes(port, data, size, confirmed || should_confirm);
    auto code = network_->error();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        if (code == LoraErrorCode::NotJoined) {
            logwarn("not-joined");
            gs->lora.joined = 0;
        }
        if (success) {
            loginfo("success!");
            gs->lora.tx_successes++;
        } else {
            logwarn("failed!");
            gs->lora.tx_failures++;
            gs->lora.confirmed_tries++;
        }
    });

    return code;
}

void LoraManager::stop() {
    if (awake_) {
        if (!network_->sleep(OneDayMs)) {
            logerror("error sleeping");
        }

        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) { gs->lora.asleep = fk_uptime(); });

        awake_ = false;
    }
}

} // namespace fk
