#pragma once

#include "hal/sd_card.h"

#if defined(__SAMD51__)

#include <SdFat.h>
#include <sdios.h>

namespace fk {

class MetalSdCard : public SdCard {
public:
    MetalSdCard();

public:
    bool begin() override;

};

}

#endif
