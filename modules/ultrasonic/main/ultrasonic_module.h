#pragma once

#include <modules/bridge/modules.h>

#include <hal/metal/sc16is740.h>
#include <line_reader.h>

class UltrasonicModule : public fk::Module {
private:
    fk::Sc16is740 bridge_;
    fk::LineReader<256> line_reader_{ &bridge_ };

public:
    UltrasonicModule();

public:
    bool initialize(fk::ModuleContext mc, fk::Pool &pool) override;
    bool service(fk::ModuleContext mc, fk::Pool &pool) override;
    bool api(fk::ModuleContext mc, fk::HttpServerConnection *connection, fk::Pool &pool) override;
    fk::ModuleReadings *take_readings(fk::ModuleContext mc, fk::Pool &pool) override;

public:
    fk::ModuleSensors const *get_sensors(fk::Pool &pool) override;
    fk::ModuleConfiguration get_configuration(fk::Pool &pool) override;

};
