#pragma once

#include <cstring>

#include "common.h"
#include "pool.h"
#include "httpd/req.h"

namespace fk {

class HttpHandler {
public:
    virtual bool handle(HttpRequest &req) = 0;

};

class HttpRoute {
private:
    const char *url_;
    HttpHandler *handler_;

public:
    HttpRoute(const char *url, HttpHandler *handler) : url_(url), handler_(handler) {
    }

public:
    virtual bool matches(const char *url) const {
        return strncmp(url_, url, strlen(url_)) == 0;
    }

    HttpHandler *handler() {
        return handler_;
    }

};

class HttpRouter {
private:
    HttpRoute *routes_[HttpMaximumRoutes] = { nullptr };

public:
    /**
     * Finds the correct handler for a URL. If no appropriate handler can be
     * found, returns nullptr.
     */
    HttpHandler *route(const char *url);

    /**
     * Registers a new route. Returns true if the route was successfully added
     * and false if an error occured, like there is no more more.
     */
    bool add_route(HttpRoute *route);

    /**
     * Returns the maximum number of routes.
     */
    constexpr size_t maximum_number_of_routes() {
        return HttpMaximumRoutes;
    }

};

}
