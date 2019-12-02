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

bool LoraManager::begin() {
    GlobalStateManager gsm;

    auto success = network_->begin();

    auto state = get_lora_state();
    if (state.asleep > 0) {
        loginfo("waking");

        if (!network_->wake()) {
            logerror("error waking");
            return false;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->lora.asleep = 0;
        });

        awake_ = true;
    }

    gsm.apply([=](GlobalState *gs) {
        gs->lora.has_module = success;
        gs->lora.joined = 0;
        gs->lora.asleep = 0;
    });

    return success;
}

static bool is_null_byte_array(uint8_t const *ptr, size_t length) {
    for (auto i = 0u; i < length; ++i) {
        if (ptr[i] != 0) {
            return false;
        }
    }
    return true;
}

bool LoraManager::join_if_necessary(Pool &pool) {
    GlobalStateManager gsm;

    auto state = get_lora_state();

    if (state.joined == 0 || state.joined > fk_uptime() || fk_uptime() - state.joined > OneDayMs) {
        if (!state.configured) {
            loginfo("no configuration");
            gsm.apply([=](GlobalState *gs) {
                gs->lora.configured = false;
                gs->lora.joined = 0;
                gs->lora.asleep = 0;
            });
            return false;
        }

        auto joined = false;

        if (!network_->resume_previous_session()) {
            if (!is_null_byte_array(state.app_key, LoraAppKeyLength)) {
                auto app_key = bytes_to_hex_string_pool(state.app_key, LoraAppKeyLength, pool);
                auto app_eui = bytes_to_hex_string_pool(state.app_eui, LoraAppEuiLength, pool);

                loginfo("joining via otaa");

                joined = network_->join(app_eui, app_key);
            }
            else if (!is_null_byte_array(state.app_session_key, LoraAppSessionKeyLength) &&
                !is_null_byte_array(state.network_session_key, LoraNetworkSessionKeyLength) &&
                !is_null_byte_array(state.device_address, LoraDeviceAddressLength)) {
                auto app_session_key = bytes_to_hex_string_pool(state.app_session_key, LoraAppSessionKeyLength, pool);
                auto network_session_key = bytes_to_hex_string_pool(state.network_session_key, LoraNetworkSessionKeyLength, pool);
                auto device_address = bytes_to_hex_string_pool(state.device_address, LoraDeviceAddressLength, pool);
                auto uplink_counter = state.uplink_counter;
                auto downlink_counter = state.downlink_counter;

                loginfo("joining via abp");

                joined = network_->join(app_session_key, network_session_key, device_address, uplink_counter, downlink_counter);
            }
            else {
                logwarn("no configuration");
            }
        }
        else {
            loginfo("resumed");

            joined = true;
        }

        gsm.apply([=](GlobalState *gs) {
            gs->lora.configured = true;
            gs->lora.has_module = true;
            gs->lora.joined = joined ? fk_uptime() : 0;
            gs->lora.asleep = 0;
        });

        awake_ = true;

        return joined;
    }

    return true;
}

LoraErrorCode LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    auto success = network_->send_bytes(port, data, size, confirmed);
    auto code = network_->error();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        if (code == LoraErrorCode::NotJoined) {
            gs->lora.joined = 0;
        }
        if (success) {
            auto uplink_counter = network_->uplink_counter();
            if (uplink_counter - last_save_ > LoraUplinksSaveFrequency) {
                if (!network_->save_state()) {
                    logerror("error saving state");
                }
                last_save_ = uplink_counter;
            }
            gs->lora.tx_successes++;
        } else {
            gs->lora.tx_failures++;
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
        gsm.apply([=](GlobalState *gs) {
            gs->lora.asleep = fk_uptime();
        });

        awake_ = false;
    }
}

} // namespace fk
