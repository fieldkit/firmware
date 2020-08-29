#pragma once

#include "networking/networking.h"

namespace fk {

class ModuleHandler : public HttpHandler {
private:
    ModulePosition bay_;

public:
    ModuleHandler(ModulePosition bay);

public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

}
