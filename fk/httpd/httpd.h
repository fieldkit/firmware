#pragma once

#include <http_parser.h>

#include "../common.h"

namespace fk {

enum class HttpRequestState {
    New,
    URL,
    Headers,
    Body,
    Consumed,
    Error,
    Done
};

class HttpHandler {
public:
    virtual bool handle() = 0;

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

class HttpRequest {
private:
    http_parser parser_;
    http_parser_settings settings_{ 0 };

    /**
     * Current state of this HTTP request. Basically set in response to the
     * parser callbacks.
     */
    HttpRequestState state_{ HttpRequestState::New };

    /**
     * URL being acted upon. Eventually this should probably be more flexible,
     * size wise. This should handle most cases initially, though.
     */
    char url_[64];

    /**
     * Previous header field parsed. This will be valid because we always
     * enforce keeping the headers in a single buffer, for now.
     */
    const char *header_name_{ nullptr };
    size_t header_name_len_{ 0 };

    /**
     * Length of this request, as supplied in the Content-Length header.
     */
    uint32_t length_;

public:
    HttpRequest();

public:
    /**
     *
     */
    void begin();

    /**
     *
     */
    int32_t parse(const char *data, size_t length);

public:
    HttpRequestState state() const {
        return state_;
    }

    bool done() const {
        return state_ == HttpRequestState::Error || state_ == HttpRequestState::Done;
    }

    bool have_headers() const {
        return state_ == HttpRequestState::Body;
    }

    bool consumed() const {
        return state_ == HttpRequestState::Consumed;
    }

    const char *url() const {
        return url_;
    }

    uint32_t length() const {
        return length_;
    }

public:
    int32_t on_message_begin();
    int32_t on_url(const char *at, size_t length);
    int32_t on_header_field(const char *at, size_t length);
    int32_t on_header_value(const char *at, size_t length);
    int32_t on_headers_complete();
    int32_t on_data(const char *at, size_t length);
    int32_t on_message_complete();

};

}
