#include <cstring>
#include <cstdlib>

#include "common.h"
#include "platform.h"
#include "httpd.h"

namespace fk {

#if defined(FK_LOG_HTTPD_VERBOSE)
#define FK_HTTPD_LOG(f, ...)    fkb_external_println(f, ## __VA_ARGS__)
#else
#define FK_HTTPD_LOG(f, ...)
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

HttpRequest::HttpRequest() {
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
    return http_parser_execute(&parser_, &settings_, data, length);
}

int32_t HttpRequest::on_message_begin() {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::URL;

    return 0;
}

int32_t HttpRequest::on_url(const char *at, size_t length) {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

    auto n = std::min(length, sizeof(url_));
    strncpy(url_, at, n);
    url_[length] = 0;

    state_ = HttpRequestState::Headers;

    return 0;
}

int32_t HttpRequest::on_header_field(const char *at, size_t length) {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

    header_name_ = at;
    header_name_len_ = length;

    return 0;
}

int32_t HttpRequest::on_header_value(const char *at, size_t length) {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

    auto n = std::min(header_name_len_, strlen(HTTP_CONTENT_LENGTH));
    if (strncasecmp(header_name_, HTTP_CONTENT_LENGTH, n) == 0) {
        length_ = atoi(at);
    }

    return 0;
}

int32_t HttpRequest::on_headers_complete() {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::Body;

    return 0;
}

int32_t HttpRequest::on_data(const char *at, size_t length) {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);
    return 0;
}

int32_t HttpRequest::on_message_complete() {
    FK_HTTPD_LOG("hi: %s", __PRETTY_FUNCTION__);

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
