#pragma once

#include "hal/gps.h"

namespace fk {

class LinuxGps : public Gps {
public:
    bool begin() override;
    bool check() override;
    bool service(GpsFix &fix) override;
    bool stop() override;

};

}
