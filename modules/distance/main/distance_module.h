#pragma once

#include <modules/bridge/modules.h>

#include <hal/metal/sc16is740.h>
#include <line_reader.h>

namespace fk {

class DistanceModule : public Module {
private:
    Sc16is740 bridge_;
    LineReader<256> line_reader_{ &bridge_ };

public:
    DistanceModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration const get_configuration(Pool &pool) override;

};

} // namespace fk
