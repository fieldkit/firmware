#pragma once

#include <modules/bridge/modules.h>

#include <hal/metal/sc16is740.h>
#include <line_reader.h>

namespace fk {

class UltrasonicModule : public Module {
private:
    Sc16is740 bridge_;
    LineReader<256> line_reader_{ &bridge_ };

public:
    UltrasonicModule();

public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override;
    ModuleReturn service(ModuleContext mc, Pool &pool) override;
    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) override;
    ModuleReadings *take_readings(ModuleContext mc, Pool &pool) override;

public:
    ModuleSensors const *get_sensors(Pool &pool) override;
    ModuleConfiguration get_configuration(Pool &pool) override;

};

} // namespace fk
