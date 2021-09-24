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

    auto has_module = network_->begin(state.frequency_band);
    auto module_state = has_module ? network_->get_state(pool) : nullptr;

    awake_ = true;

    if (has_module) {
        // Just doing this to log errors during startup.
        verify_configuration(pool);
    }

    return gsm.apply([&](GlobalState *gs) {
        // Note that this only happens when a module comes or goes.
        if (gs->lora.has_module != has_module) {
            gs->lora.has_module = has_module;
            gs->lora.joined = 0;
            gs->lora.asleep = 0;
            gs->lora.tx_confirmed_tries = 0;
            if (module_state != nullptr) {
                memcpy(gs->lora.device_address, module_state->device_address, sizeof(gs->lora.device_address));
                gs->lora.uplink_counter = module_state->uplink_counter;
                gs->lora.downlink_counter = module_state->downlink_counter;
            } else {
                bzero(gs->lora.device_address, sizeof(gs->lora.device_address));
            }
            auto device_address_hex = bytes_to_hex_string_pool(gs->lora.device_address, sizeof(gs->lora.device_address), pool);
            loginfo("has-module: %d device-address: %s", has_module, device_address_hex);
#if defined(FK_LORA_FULL_RESET)
            gs->lora.tx_confirmed_failures = 0;
            gs->lora.activity = 0;
            gs->lora.confirmed = 0;
            gs->lora.tx_successes = 0;
            gs->lora.tx_failures = 0;
#endif
        }

        return has_module;
    });
}

bool LoraManager::factory_reset() {
    return network_->factory_reset();
}

bool LoraManager::verify_configuration(Pool &pool) {
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

static void update_lora_status(LoraState &lora, Rn2903State const *rn) {
    lora.uplink_counter = rn->uplink_counter;
    lora.downlink_counter = rn->downlink_counter;
    FK_ASSERT(sizeof(lora.device_address) == sizeof(rn->device_address));
    memcpy(lora.device_address, rn->device_address, sizeof(lora.device_address));
}

bool LoraManager::join_if_necessary(Pool &pool) {
    if (!verify_configuration(pool)) {
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
    auto joined_otaa = false;
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
        joined_otaa = joined;
    } else {
        loginfo("joining via stored abp");

        joined = network_->join_resume();
    }

    auto state_after = network_->get_state(pool);

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        gs->lora.has_module = true;
        gs->lora.joined = joined ? fk_uptime() : 0;
        gs->lora.activity = fk_uptime();
        gs->lora.asleep = 0;
        if (joined_otaa) {
            gs->lora.tx_confirmed_tries = 0;
            gs->lora.tx_confirmed_failures = 0;
        }
        update_lora_status(gs->lora, state_after);
    });

    awake_ = true;

    return joined;
}

struct Confirmation {
    bool confirmed;
    bool retry;

    Confirmation() : confirmed(false), retry(false) {
    }

    Confirmation(bool confirmed, bool retry) : confirmed(confirmed), retry(retry) {
    }
};

static Confirmation get_should_confirm() {
    auto lora = get_lora_global_state();

    // Handle confirmations every LoraConfirmEveryTx messages.
    if (LoraConfirmEveryTx > 0 && lora.tx_total > 0) {
        if (LoraConfirmEveryTx > 0 && (lora.tx_total % LoraConfirmEveryTx) == 0) {
            loginfo("confirming every %d tx", LoraConfirmEveryTx);
            lora.tx_confirmed_tries = 0;
            return Confirmation{ true, false };
        }
    }

    // If we're configured to send a confirmation every LoraConfirmEveryMinutes minutes.
    if (LoraConfirmEveryMinutes > 0) {
        auto minutes_elapsed = (fk_uptime() - lora.confirmed) / OneMinuteMs;
        if (minutes_elapsed > LoraConfirmEveryMinutes) {
            loginfo("%d confirmed every %d minutes (%d)", LoraConfirmEveryMinutes, minutes_elapsed);
            lora.tx_confirmed_tries = 0;
            return Confirmation{ true, false };
        }
    }

    // Did our last confirmed send fail?
    if (lora.tx_confirmed_tries > 0) {
        // This retries a failed confirmation up to LoraConfirmedRetry times.
        if (LoraConfirmedRetries > 0) {
            if (lora.tx_confirmed_tries == LoraConfirmedRetries) {
                lora.tx_confirmed_tries = 0;
                lora.tx_confirmed_failures++;
                logwarn("%d confirmed tries failed", lora.tx_confirmed_tries);
                return Confirmation{};
            }
            loginfo("%d confirmed tries", lora.tx_confirmed_tries);
            return Confirmation{ true, true };
        } else {
            lora.tx_confirmed_tries = 0;
            lora.tx_confirmed_failures++;
            logwarn("%d confirmed tries failed", lora.tx_confirmed_tries);
            return Confirmation{};
        }
    }

    return Confirmation{};
}

enum class PostSendAction {
    Success,
    Save,
    Failure,
    Rejoin,
};

static PostSendAction return_rejoin(GlobalState *gs) {
    gs->lora.joined = 0;
    gs->lora.tx_confirmed_tries = 0;
    gs->lora.tx_confirmed_failures = 0;
    return PostSendAction::Rejoin;
}

static PostSendAction return_save(GlobalState *gs) {
    gs->lora.state_saved = fk_uptime();
    return PostSendAction::Save;
}

static PostSendAction update_gs_after_send(GlobalState *gs, Confirmation confirmed, bool module_error, LoraErrorCode lora_error) {
    auto uptime = fk_uptime();

    gs->lora.activity = uptime;
    gs->lora.tx_total++;

    if (module_error) {
        logerror("module-io: error");
        gs->lora.tx_failures++;

        return PostSendAction::Failure;
    }

    switch (lora_error) {
    case LoraErrorCode::ModuleIO: {
        // We should never get here, should return as a module error above.
        logwarn("module-io: error");

        return PostSendAction::Failure;
    }
    case LoraErrorCode::NotJoined: {
        logwarn("joined: error");

        return return_rejoin(gs);
    }
    case LoraErrorCode::Mac: {
        // It would be unexpected to get this on an unconfirmed message.
        gs->lora.tx_failures++;

        if (confirmed.confirmed) {
            if (confirmed.retry) {
                logwarn("mac-error: failed retry %d/%d", gs->lora.tx_confirmed_tries, gs->lora.tx_confirmed_failures);
            } else {
                logwarn("mac-error: failed confirmed %d/%d", gs->lora.tx_confirmed_tries, gs->lora.tx_confirmed_failures);
            }

            if (LoraConfirmedRetries > 0) {
                gs->lora.tx_confirmed_tries++;
            } else {
                gs->lora.tx_confirmed_failures++;
                if (gs->lora.tx_confirmed_failures == LoraFailuresBeforeRejoin) {
                    logwarn("%d confirmed failures, triggering rejoin.", gs->lora.tx_confirmed_failures);

                    return return_rejoin(gs);
                }
            }
        } else {
            logwarn("mac-error: unexpected");
        }

        break;
    }
    default: {
        loginfo("success!");

        gs->lora.tx_successes++;
        gs->lora.tx_confirmed_tries = 0;
        gs->lora.tx_confirmed_failures = 0;

        if (confirmed.confirmed) {
            loginfo("saving confirmed");

            gs->lora.confirmed = uptime;

            return return_save(gs);
        } else if (LoraSaveEveryTx > 0) {
            // We always save RN state after a successful confirmed message,
            // this saves the uplink counters periodically.
            if ((gs->lora.tx_total % LoraSaveEveryTx) == 0) {
                loginfo("saving every %d txs", LoraSaveEveryTx);

                return return_save(gs);
            }
        }

        break;
    }
    }

    return PostSendAction::Success;
}

bool LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size, Pool &pool) {
    auto confirmed = get_should_confirm();
    auto module_error = !network_->send_bytes(port, data, size, confirmed.confirmed);
    auto lora_error = network_->error();

    GlobalStateManager gsm;
    auto action =
        gsm.apply_r<PostSendAction>([=](GlobalState *gs) { return update_gs_after_send(gs, confirmed, module_error, lora_error); });

    switch (action) {
    case PostSendAction::Failure:
        return false;
    case PostSendAction::Save:
        if (!network_->save_state()) {
            logwarn("saving state");
        }
        return true;
    case PostSendAction::Rejoin:
        logwarn("factory reset module to trigger join");
        return network_->factory_reset();
    default:
        return true;
    }
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
