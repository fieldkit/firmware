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
        gs->lora.has_module = success;
        gs->lora.joined = 0;
        gs->lora.asleep = 0;

        if (success) {
            // auto device_eui = network_->device_eui();
            // memcpy(gs->lora.device_eui, device_eui, LoraDeviceEuiLength);
            // loginfo("(loaded) lora device eui: %s", bytes_to_hex_string_pool(device_eui, LoraDeviceEuiLength, pool));
        } else {
            memzero(gs->lora.device_eui, LoraDeviceEuiLength);
        }
    });

    return success;
}

bool LoraManager::factory_reset() {
    return network_->factory_reset();
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
    auto state = get_lora_global_state();
    auto module_state = network_->get_state(pool);

    if (is_null_byte_array(state.app_eui, LoraAppEuiLength)) {
        logerror("state missing app-eui");
        return false;
    }

    auto joined = false;
    if (is_null_byte_array(module_state->device_address, LoraDeviceAddressLength)) {
        loginfo("module missing devaddr, joining via otaa");
        joined = network_->join(bytes_to_hex_string_pool(state.app_eui, LoraAppEuiLength, pool),
                                bytes_to_hex_string_pool(state.app_key, LoraAppKeyLength, pool));
    } else {
        loginfo("joining via stored abp");
        joined = network_->join_resume();
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

LoraErrorCode LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed, Pool &pool) {
    auto success = network_->send_bytes(port, data, size, confirmed);
    auto code = network_->error();

    GlobalStateManager gsm;
    gsm.apply([=](GlobalState *gs) {
        if (code == LoraErrorCode::NotJoined) {
            gs->lora.joined = 0;
        }
        if (success) {
            gs->lora.tx_successes++;
        } else {
            gs->lora.tx_failures++;
        }
    });

    /*
    if (code == LoraErrorCode::DataLength) {
        // Show module state in logs.
        network_->get_state(pool);
    }
    */

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
