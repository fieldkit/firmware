#pragma once

#if defined(ARDUINO)

#include "common.h"

namespace fk {

class LoraModule {
public:
    LoraModule();

public:
    bool begin();

};

}

#endif
