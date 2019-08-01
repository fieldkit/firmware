#pragma once

#include <modules.h>

class RandomModule : public fk::Module {
public:
    bool initialize(fk::ModuleContext &mc) override;
    bool take_readings(fk::ModuleContext &mc) override;

};
