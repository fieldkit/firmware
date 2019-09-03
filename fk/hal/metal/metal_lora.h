#pragma once

#include "hal/lora.h"
#include "hal/metal/rn2903.h"

#if defined(ARDUINO)

namespace fk {

class Rn2903LoraNetwork : public LoraNetwork {
private:
    Rn2903 rn2903_;

public:
    Rn2903LoraNetwork();

public:
    bool begin() override;
    bool power(bool on) override;
    bool sleep(uint32_t ms) override;
    bool send_bytes(uint8_t const *data, size_t size) override;

public:
    bool show_status();

};

}

#endif
