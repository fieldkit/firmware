#pragma once

#include "lora_frequency.h"
#include "common.h"

namespace fk {

struct LoraState {
    lora_frequency_t frequency_band{ LoraDefaultFrequency };
    uint8_t device_eui[LoraDeviceEuiLength];
    uint8_t app_key[LoraAppKeyLength];
    uint8_t join_eui[LoraJoinEuiLength];
    uint8_t device_address[LoraDeviceAddressLength];
    uint32_t uplink_counter;
    uint32_t downlink_counter;
    uint32_t rx_delay_1{ 0 };
    uint32_t rx_delay_2{ 0 };
#if defined(FK_LORA_ABP)
    uint8_t network_session_key[LoraNetworkSessionKeyLength];
    uint8_t app_session_key[LoraAppSessionKeyLength];
#endif
    bool has_module{ false };
    uint32_t joined{ 0 };
    uint32_t asleep{ 0 };
    uint32_t activity{ 0 };
    uint32_t confirmed{ 0 };
    uint32_t tx_total{ 0 };
    uint32_t tx_successes{ 0 };
    uint32_t tx_failures{ 0 };
    uint32_t tx_confirmed_tries{ 0 };
    uint32_t tx_confirmed_failures{ 0 };
    uint32_t state_saved{ 0 };
    uint32_t sessions{ 0 };
};

} // namespace fk