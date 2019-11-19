#pragma once

#include "networking/networking.h"

namespace fk {

class ModuleHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

}
