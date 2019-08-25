#pragma once

#include "networking/networking.h"

namespace fk {

class ApiHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req, Pool &pool) override;

};

}
