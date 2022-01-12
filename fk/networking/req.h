#pragma once

#include <cstring>
#include <http_parser.h>

#include <fk-app-protocol.h>

#include "common.h"
#include "pool.h"
#include "io.h"
#include "config.h"
#include "networking/url_parser.h"

namespace fk {

class Connection;

enum class HttpRequestState { New, URL, Headers, Body, Consumed, Error, Done };

enum class WellKnownContentType { Unknown, TextPlain, ApplicationOctetStream, ApplicationFkHttp };

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
    char url_[HttpMaximumUrlLength];

    /**
     * The parsed URL.
     */
    UrlParser url_parser_;

    /**
     * Previous header field parsed. This will be valid because we always
     * enforce keeping the headers in a single buffer, for now.
     */
    const char *header_name_{ nullptr };
    size_t header_name_len_{ 0 };

    /**
     * Length of this request, as supplied in the Content-Length header.
     */
    uint32_t length_{ 0 };

    /**
     * Memory pool to allocate request data from.
     */
    Pool *pool_;

    /**
     * A well-known content type.
     */
    WellKnownContentType content_type_{ WellKnownContentType::Unknown };

    uint8_t const *buffered_body_{ nullptr };
    size_t buffered_body_length_{ 0 };
    const char *user_agent_{ nullptr };

public:
    HttpRequest(Pool *pool);

public:
    /**
     * Resets the state for this HttpRequest. This includes emptying buffers and
     * starting the parser logic over again.
     */
    void begin();

    /**
     * Parse data received from the client.
     */
    int32_t parse(const char *data, size_t length);

public:
    /**
     * Returns the current state machine state of the request.
     */
    HttpRequestState state() const {
        return state_;
    }

    /**
     * Returns true if the handling for this request is done and the associated
     * resources can be freed.
     */
    bool done() const {
        return state_ == HttpRequestState::Error || state_ == HttpRequestState::Done;
    }

    /**
     * Returns true if the headers for the request have come in and the body has
     * begun, if one is present.
     */
    bool have_headers() const {
        return state_ == HttpRequestState::Body || state_ == HttpRequestState::Consumed;
    }

    /**
     * Returns true if the request has been completely consumed.
     */
    bool consumed() const {
        return state_ == HttpRequestState::Consumed;
    }

    /**
     * Returns true if the request has been completely consumed.
     */
    void finished() {
        state_ = HttpRequestState::Done;
    }

    /**
     * Returns the URL being operated in in this HttpRequest. This is only valid
     * after headers have been consumed.
     */
    const char *url() const {
        return url_;
    }

    const char *query_string() const {
        return url_parser_.query_string();
    }

    /**
     * Returns the URL parser with the parsed URL, assuming the URL has been received.
     */
    UrlParser &url_parser() {
        return url_parser_;
    };

    /**
     * Returns the length, in bytes, of the body of this request. This is only
     * valid after headers have been consumed.
     */
    uint32_t length() const {
        return length_;
    }

    /**
     * Returns the Content-Type of this request.
     */
    WellKnownContentType content_type() const {
        return content_type_;
    }

    /**
     * Returns true if the HTTP method is GET.
     */
    bool is_get_method() const {
        return parser_.method == HTTP_GET;
    }

    /**
     * Returns true if the HTTP method is HEAD.
     */
    bool is_head_method() const {
        return parser_.method == HTTP_HEAD;
    }

    /**
     * Returns the User-Agent header.
     */
    const char *user_agent() const {
        return user_agent_;
    }

    /**
     * HTTP status parsed from the response.
     */
    int32_t status_code() const {
        return parser_.status_code;
    }

public:
    /**
     * This is a long story.
     */
    int32_t read_buffered_body(uint8_t *buffer, size_t size);

    int32_t buffered_body_length() const {
        return buffered_body_length_;
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

} // namespace fk
