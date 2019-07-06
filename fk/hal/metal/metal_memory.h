#pragma once

#if defined(ARDUINO)

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"

#include <SerialFlash.h>

namespace fk {

class MetalDataMemory : public DataMemory {
private:

public:
    MetalDataMemory();

public:
    bool begin() override;

};

}

#endif
