#pragma once

#include "hal/lora.h"
#include "pool.h"
#include "state.h"

namespace fk {

class LoraManager {
private:
    LoraNetwork *network_{ nullptr };
    bool awake_{ false };

public:
    explicit LoraManager(LoraNetwork *network);

public:
    bool begin(Pool &pool);
    bool factory_reset();
    bool power_cycle();
    bool join_if_necessary(Pool &pool);
    void stop();

public:
    bool send_bytes(uint8_t port, uint8_t const *data, size_t size, Pool &pool);

private:
    bool verify_configuration(LoraState const &state, uint8_t const *device_eui, Pool &pool);
    void verify_rx_delays(Rn2903State const *rn, Pool &pool);

private:
    void update_lora_status(LoraState &lora, Rn2903State const *rn);
};

} // namespace fk
