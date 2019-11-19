#pragma once

#include "networking/networking.h"

namespace fk {

class ApiHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

};

}
