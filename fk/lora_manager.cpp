#include "lora_manager.h"
#include "state_ref.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

uint32_t LoraManager::joined_ = 0;
uint32_t LoraManager::asleep_ = 0;

LoraManager::LoraManager(LoraNetwork *network) : network_(network) {
}

bool LoraManager::available() {
    auto gs = get_global_state_ro();
    return gs.get()->lora.configured;
}

bool LoraManager::join_if_necessary(Pool &pool) {
    if (joined_ == 0 || joined_ > fk_uptime() || fk_uptime() - joined_ > OneDayMs) {
        auto gs = get_global_state_ro();
        if (!gs.get()->lora.configured) {
            loginfo("no configuration");
            return false;
        }

        if (!network_->begin()) {
            logwarn("missing module");
            return false;
        }

        auto app_key = bytes_to_hex_string_pool(gs.get()->lora.app_key, LoraAppKeyLength, pool);
        auto app_eui = bytes_to_hex_string_pool(gs.get()->lora.app_eui, LoraAppEuiLength, pool);

        if (!network_->join(app_eui, app_key)) {
            return false;
        }

        joined_ = fk_uptime();
        asleep_ = 0;

        return true;
    }

    if (asleep_ > 0) {
        if (!network_->wake()) {
            logerror("error waking");
            return false;
        }

        asleep_ = 0;
    }

    return true;
}

bool LoraManager::send_bytes(uint8_t port, uint8_t const *data, size_t size) {
    if (network_->send_bytes(port, data, size)) {
        return true;
    }

    switch (network_->error()) {
    case LoraErrorCode::NotJoined: {
        loginfo("rejoining");
        joined_ = 0;
        break;
    }
    default: {
        logerror("tx failed");
        break;
    }
    }

    return false;
}

void LoraManager::stop() {
    if (!network_->sleep(OneHourMs)) {
        logerror("error sleeping");
    }
}

} // namespace fk
