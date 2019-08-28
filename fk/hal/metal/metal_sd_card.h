#pragma once

#include "hal/sd_card.h"

namespace fk {

class MetalSdCard : public SdCard {
public:
    MetalSdCard();

public:
    bool begin() override;

};

}
