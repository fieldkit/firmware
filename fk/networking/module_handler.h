#pragma once

#include "networking/networking.h"

namespace fk {

class ModuleHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req) override;

};

}
