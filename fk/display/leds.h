#pragma once

#include "common.h"

namespace fk {

class LedsController {
private:
    uint32_t version_{ 0 };

public:
    LedsController();

public:
    bool begin();
    void tick();

private:

};

} //namespace fk
