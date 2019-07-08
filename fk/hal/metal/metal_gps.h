#pragma once

#include "hal/gps.h"

#if defined(ARDUINO)

#include <TinyGPS.h>

namespace fk {

class MetalGps : public Gps {
private:
    TinyGPS gps_;

public:
    bool begin() override;
    bool check() override;
    bool service(GpsFix &fix) override;

};

}

#endif
