#pragma once

#include "networking/networking.h"

namespace fk {

class ApiHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;

private:
    void adjust_time_if_necessary(fk_app_HttpQuery const *query);
    void adjust_location_if_necessary(fk_app_HttpQuery const *query);

};

}
