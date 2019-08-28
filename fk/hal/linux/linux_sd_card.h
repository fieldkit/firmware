#pragma once

#include "hal/sd_card.h"

namespace fk {

class LinuxSdCard : public SdCard {
public:
    LinuxSdCard();

public:
    bool begin() override;

};

}
