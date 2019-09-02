#pragma once

#include "hal/lora.h"
#include "hal/metal/rn2903.h"

namespace fk {

class Rn2903LoraNetwork : public LoraNetwork {
public:
    Rn2903LoraNetwork();

public:
    bool begin() override;
};

}
