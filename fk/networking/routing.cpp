#include "networking/routing.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

bool HttpRoute::matches(const char *url) const {
    auto route_len = strlen(url_);
    if (route_len != strlen(url)) {
        return false;
    }
    return strncmp(url_, url, route_len) == 0;
}

HttpHandler *HttpRouter::route(const char *url) {
    if (url == nullptr) {
        return nullptr;
    }
    for (auto i = 0u; i < maximum_number_of_routes(); ++i) {
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
    for (auto i = 0u; i < maximum_number_of_routes(); ++i) {
        if (routes_[i] == nullptr) {
            routes_[i] = route;
            return true;
        }
    }

    FK_ASSERT(false);

    return false;
}

} // namespace fk
