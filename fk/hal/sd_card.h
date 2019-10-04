#pragma once

#include "circular_buffer.h"

namespace fk {

class SdCard {
public:
    virtual bool begin() = 0;

public:
    virtual bool append_logs(circular_buffer<char> &buffer) = 0;

};

SdCard *get_sd_card();

}
