#pragma once

#include <modules.h>

class WaterModule : public fk::Module {
public:
    bool initialize(fk::ModuleContext mc) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc) override;

};
