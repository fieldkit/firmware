#pragma once

#include "hal/lora.h"
#include "pool.h"

namespace fk {

class LoraManager {
private:
    LoraNetwork *network_;
    // These statics are temporary.
    static uint32_t joined_;
    static uint32_t asleep_;

public:
    LoraManager(LoraNetwork *network);

public:
    bool available();
    bool join_if_necessary(Pool &pool);
    bool send_bytes(uint8_t port, uint8_t const *data, size_t size);
    void stop();

};

}
