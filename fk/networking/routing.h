#pragma once

#include <cstring>

#include "common.h"
#include "pool.h"
#include "networking/req.h"

namespace fk {

class HttpServerConnection;

/**
 * Interface to be overriden for custom HTTP route handlers. These are then
 * registered with an HttpRoute in the HttpRouter.
 */
class HttpHandler {
public:
    /**
     * Handle an incoming HTTP request.
     */
    virtual bool handle(HttpServerConnection *connection, Pool &pool) = 0;
};

/**
 * Defines the relationship between a URL and a specific HttpHandler. May
 * include fancier forms of matching, like eventually headers and the like.
 * Right now we do simple URL matching.
 */
class HttpRoute {
private:
    const char *url_;
    HttpHandler *handler_;

public:
    HttpRoute(const char *url, HttpHandler *handler) : url_(url), handler_(handler) {
    }

public:
    /**
     * Test to see if the URL matches this route.
     */
    virtual bool matches(const char *url) const;

    /**
     * The HttpHandler that should handle the requests that match this route.
     */
    HttpHandler *handler() {
        return handler_;
    }
};

/**
 * Manages all available HTTP routes and oversees matching incoming URLs to a
 * specific HttpHandler.
 */
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

} // namespace fk
