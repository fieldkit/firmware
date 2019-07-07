#pragma once

#include "httpd/httpd.h"

namespace fk {

class DownloadHandler : public HttpHandler {
public:
    bool handle(HttpRequest &req) override;

};

}
