#pragma once

#if defined(ARDUINO)

#include <TinyGPS.h>

#include "hal/gps.h"
#include "config.h"

namespace fk {

class MetalGps : public Gps {
private:
    char buffer[GpsDebuggingBuffer];
    size_t i = 0;
    TinyGPS gps_;

public:
    bool begin() override;
    bool check() override;
    bool service(GpsFix &fix) override;

};

}

#endif
