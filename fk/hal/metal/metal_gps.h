#pragma once

#if defined(__SAMD51__)

#include "config.h"
#include "hal/gps.h"

#include <TinyGPS.h>

namespace fk {

class MetalGps : public Gps {
private:
    char buffer_[GpsDebuggingBuffer];
    size_t position_ = 0;
    TinyGPS gps_;

public:
    bool begin() override;
    bool service(GpsFix &fix) override;
    bool stop() override;
};

} // namespace fk

#endif
