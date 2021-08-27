#pragma once

#include "hal/lora.h"
#include "pool.h"

namespace fk {

class LoraManager {
private:
    LoraNetwork *network_{ nullptr };
    bool awake_{ false };

public:
    explicit LoraManager(LoraNetwork *network);

public:
    bool begin(Pool &pool);
    bool available();
    bool factory_reset();
    bool join_if_necessary(Pool &pool);
    void stop();

public:
    bool configure_tx(uint8_t power_index, uint8_t data_rate);
    LoraErrorCode send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed, Pool &pool);
};

} // namespace fk
