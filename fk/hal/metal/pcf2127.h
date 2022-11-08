#pragma once
#if defined(__SAMD51__)

#include "common.h"
#include "hal/board.h"
#include "io.h"

#include <lwcron/lwcron.h>

namespace fk {

using DateTime = lwcron::DateTime;

class Pcf2127 {
public:
    bool configure();
    bool adjust(DateTime now);
    bool read(DateTime &time);

private:
    bool read_register(uint8_t reg, uint8_t &value);
    bool read_buffer(uint8_t reg, uint8_t *value, size_t size);
};

} // namespace fk

#endif
