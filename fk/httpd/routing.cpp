#include "routing.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

HttpHandler *HttpRouter::route(const char *url) {
    for (auto i = (size_t)0; i < maximum_number_of_routes(); ++i) {
        if (routes_[i] == nullptr) {
            return nullptr;
        }
        if (routes_[i]->matches(url)) {
            return routes_[i]->handler();
        }
    }
    return nullptr;
}

bool HttpRouter::add_route(HttpRoute *route) {
    for (auto i = (size_t)0; i < maximum_number_of_routes(); ++i) {
        if (routes_[i] == nullptr) {
            routes_[i] = route;
            return true;
        }
    }
    return false;
}

}
