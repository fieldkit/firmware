#include <cstring>
#include <cstdlib>
#include <memory>

#include <fk-app-protocol.h>

#include "common.h"
#include "platform.h"
#include "httpd.h"

namespace fk {

#define loginfo(f, ...)       loginfof("httpd", f, ##__VA_ARGS__)
#define logerror(f, ...)      logerrorf("httpd", f, ##__VA_ARGS__)

#if true || defined(FK_LOG_HTTPD_VERBOSE)
#define logverbose(f, ...)    loginfo(f, ##__VA_ARGS__)
#else
#define logverbose(f, ...)
#endif

constexpr const char *HTTP_CONTENT_LENGTH = "Content-Length";

static inline HttpRequest *get_object(http_parser* parser) {
    return reinterpret_cast<HttpRequest*>(parser->data);
}

static int http_on_message_begin_callback(http_parser* parser) {
    return get_object(parser)->on_message_begin();
}

static int http_url_callback(http_parser* parser, const char *at, size_t length) {
    return get_object(parser)->on_url(at, length);
}

static int http_header_field_callback(http_parser* parser, const char *at, size_t length) {
    return get_object(parser)->on_header_field(at, length);
}

static int http_header_value_callback(http_parser* parser, const char *at, size_t length) {
    return get_object(parser)->on_header_value(at, length);
}

static int http_headers_complete_callback(http_parser* parser) {
    return get_object(parser)->on_headers_complete();
}

static int http_body_callback(http_parser* parser, const char *at, size_t length) {
    return get_object(parser)->on_data(at, length);
}

static int http_message_complete_callback(http_parser* parser) {
    return get_object(parser)->on_message_complete();
}

HttpRequest::HttpRequest(Pool *pool) : pool_(pool) {
    begin();
}

void HttpRequest::begin() {
    settings_.on_url = http_url_callback;
    settings_.on_message_begin = http_on_message_begin_callback;
    settings_.on_header_field = http_header_field_callback;
    settings_.on_header_value = http_header_value_callback;
    settings_.on_headers_complete = http_headers_complete_callback;
    settings_.on_body = http_body_callback;
    settings_.on_message_complete = http_message_complete_callback;

    http_parser_init(&parser_, HTTP_REQUEST);
    http_parser_set_max_header_size(1024);

    parser_.data = this;
}

int32_t HttpRequest::parse(const char *data, size_t length) {
    http_parser_execute(&parser_, &settings_, data, length);

    if (parser_.http_errno > 0) {
        // NOTE: The caller always adds a NULL terminator to this.
        if (strlen(data) > 0) {
            fkerror("parser: '%s'", data);
        }
        auto err = (enum http_errno)parser_.http_errno;
        fkerror("parser: %s: %s", http_errno_name(err), http_errno_description(err));
        return parser_.http_errno;
    }

    return 0;
}

int32_t HttpRequest::on_message_begin() {
    logverbose("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::URL;

    return 0;
}

int32_t HttpRequest::on_url(const char *at, size_t length) {
    logverbose("%s", __PRETTY_FUNCTION__);

    auto n = std::min(length, sizeof(url_));
    strncpy(url_, at, n);
    url_[length] = 0;

    state_ = HttpRequestState::Headers;

    return 0;
}

int32_t HttpRequest::on_header_field(const char *at, size_t length) {
    logverbose("%s", __PRETTY_FUNCTION__);

    header_name_ = at;
    header_name_len_ = length;

    return 0;
}

int32_t HttpRequest::on_header_value(const char *at, size_t length) {
    logverbose("%s", __PRETTY_FUNCTION__);

    if (false) {
        auto name = pool_->strndup(header_name_, header_name_len_);
        auto value = pool_->strndup(at, length);
        loginfo("%s = %s", name, value);
    }

    auto n = std::min(header_name_len_, strlen(HTTP_CONTENT_LENGTH));
    if (strncasecmp(header_name_, HTTP_CONTENT_LENGTH, n) == 0) {
        length_ = atoi(at);
    }

    return 0;
}

int32_t HttpRequest::on_headers_complete() {
    logverbose("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::Body;

    return 0;
}

int32_t HttpRequest::on_data(const char *at, size_t length) {
    logverbose("%s(0x%p, %zu)", __PRETTY_FUNCTION__, at, length);

    if (length_ == length) {
        auto fields = fk_app_WireMessageQuery_fields;
        auto query = (fk_app_WireMessageQuery *)pool_->decode(fields, (uint8_t *)at, length, sizeof(fk_app_WireMessageQuery));
        FK_ASSERT(query != nullptr);
    }

    return 0;
}

int32_t HttpRequest::on_message_complete() {
    logverbose("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::Consumed;

    return 0;
}

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
