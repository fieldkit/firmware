#pragma once

namespace fk {

class SdCard {
public:
    virtual bool begin() = 0;

};

SdCard *get_sd_card();

}
