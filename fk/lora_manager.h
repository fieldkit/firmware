#pragma once

#include "hal/lora.h"
#include "pool.h"

namespace fk {

class LoraManager {
private:
    LoraNetwork *network_;
    bool awake_{ false };

public:
    LoraManager(LoraNetwork *network);

public:
    bool begin();
    bool available();
    bool join_if_necessary(Pool &pool);
    LoraErrorCode send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed = false);
    void stop();

};

}
