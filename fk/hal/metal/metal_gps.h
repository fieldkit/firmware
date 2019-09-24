#pragma once

#if defined(ARDUINO)

#include "hal/gps.h"
#include "config.h"

#include <TinyGPS.h>

namespace fk {

class MetalGps : public Gps {
private:
    char buffer_[GpsDebuggingBuffer];
    size_t position_ = 0;
    TinyGPS gps_;

public:
    bool begin() override;
    bool check() override;
    bool service(GpsFix &fix) override;
    bool stop() override;

};

}

#endif
