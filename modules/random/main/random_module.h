#pragma once

#include <modules.h>

class RandomModule : public fk::Module {
public:
    bool initialize(fk::ModuleContext mc) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc) override;

};
