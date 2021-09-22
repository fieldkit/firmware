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

    auto state = get_lora_global_state();
    if (state.asleep > 0) {
        loginfo("waking");

        if (!network_->wake()) {
            logerror("waking. RN module HUP?");
            return false;
        }
    }

    auto success = network_->begin();

    awake_ = true;

    gsm.apply([=](GlobalState *gs) {
        if (gs->lora.has_module != success) {
            loginfo("has-module: %d", success);
            gs->lora.has_module = success;
            gs->lora.joined = 0;
            gs->lora.asleep = 0;
            /*
            gs->lora.activity = 0;
            gs->lora.confirmed = 0;
            gs->lora.reply_failures = 0;
            gs->lora.tx_successes = 0;
            gs->lora.tx_failures = 0;
            gs->lora.confirmed_tries = 0;
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
    if (state.joined > 0) {
        loginfo("already joined");
        return true;
    }

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

static bool get_should_confirm() {
    auto lora = get_lora_global_state();
    if (lora.tx_successes > 0) {
        if (LoraConfirmEveryTx > 0 && (lora.tx_successes % LoraConfirmEveryTx) == 0) {
            loginfo("confirming every %d tx", LoraConfirmEveryTx);
            return true;
        }
    }
    if (lora.confirmed_tries > 0) {
        if (lora.confirmed_tries == 3) {
            lora.confirmed_tries = 0;
            logwarn("%d confirmed tries failed", lora.confirmed_tries);
            return false;
        }
        loginfo("%d confirmed tries", lora.confirmed_tries);
        return true;
    }
    if (lora.confirmed > 0) {
        auto minutes_elapsed = (fk_uptime() - lora.confirmed) / OneMinuteMs;
        if (minutes_elapsed > LoraConfirmEveryMinutes) {
            loginfo("%d confirmed every %d minutes (%d)", LoraConfirmEveryMinutes, minutes_elapsed);
            return true;
        }
    }
    return false;
}

bool LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size, Pool &pool) {
    auto confirmed = get_should_confirm();
    auto module_error = !network_->send_bytes(port, data, size, confirmed);

    GlobalStateManager gsm;
    return gsm.apply([=](GlobalState *gs) {
        gs->lora.activity = fk_uptime();

        if (module_error) {
            logerror("module-io-error");
            gs->lora.tx_failures++;
            return false;
        }

        switch (network_->error()) {
        case LoraErrorCode::ModuleIO: {
            // We should never get here, should return as a module error above.
            logwarn("module-io-error");
            return false;
        }
        case LoraErrorCode::Mac: {
            // It would be unexpected to get this on an unconfirmed message.
            if (confirmed) {
                logwarn("mac-error");
                gs->lora.confirmed_tries++;
            } else {
                logwarn("mac-error-unexpected");
            }
            gs->lora.tx_failures++;
            break;
        }
        case LoraErrorCode::NotJoined: {
            // Force a rejoin some time in the future.
            logwarn("not-joined-error");
            gs->lora.joined = 0;
            break;
        }
        default: {
            loginfo("success!");
            gs->lora.tx_successes++;
            if (confirmed) {
                gs->lora.confirmed = fk_uptime();
            } else if (LoraSaveEveryTx > 0) {
                // We always save ND state after a confirmed message, this saves
                // the uplink counters periodically.
                if ((gs->lora.tx_successes % LoraSaveEveryTx) == 0) {
                    loginfo("saving every %d txs", LoraSaveEveryTx);
                    if (!network_->save_state()) {
                        logwarn("saving state");
                    }
                }
            }
            break;
        }
        }

        return true;
    });
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
