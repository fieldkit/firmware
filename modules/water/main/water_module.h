#pragma once

#include <modules.h>

#include "oem_atlas.h"

class WaterModule : public fk::Module {
private:
    fk::AtlasSensorType type_;

public:
    bool initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleSensors const *get_sensors(fk::ModuleContext mc, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

};
