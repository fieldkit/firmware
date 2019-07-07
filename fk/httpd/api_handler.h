#pragma once

#include "httpd/httpd.h"

namespace fk {

class ApiHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req) override;

};

}
