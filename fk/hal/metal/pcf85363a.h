#pragma once
#if defined(__SAMD51__)

#include "common.h"
#include "hal/board.h"
#include "io.h"

#include <lwcron/lwcron.h>

namespace fk {

using DateTime = lwcron::DateTime;

class Pcf85363a {
private:
    constexpr static uint8_t Address = 0x51u;

public:
    bool configure();
    bool adjust(DateTime now);
    bool read(DateTime &time);
};

} // namespace fk

#endif
