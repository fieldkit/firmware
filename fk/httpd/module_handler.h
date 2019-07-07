#pragma once

#include "httpd/httpd.h"

namespace fk {

class ModuleHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req) override;

};

}
