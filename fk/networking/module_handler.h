#pragma once

#include "networking/networking.h"

namespace fk {

class ModuleHandler : public HttpHandler {
public:
    bool handle(Connection *connection, Pool &pool) override;

};

}
