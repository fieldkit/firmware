#pragma once

#include "networking/networking.h"

namespace fk {

class ApiHandler : public HttpHandler {
public:
    bool handle(Connection *connection, Pool &pool) override;

};

}
