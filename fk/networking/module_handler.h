#pragma once

#include "networking/networking.h"

namespace fk {

class ModuleHandler : public HttpHandler {
private:
    uint8_t bay_;

public:
    ModuleHandler(uint8_t bay);

public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

}
