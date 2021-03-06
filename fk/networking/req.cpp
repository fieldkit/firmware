#include <cstring>
#include <cstdlib>
#include <memory>

#include "common.h"
#include "platform.h"
#include "config.h"
#include "networking/networking.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("httpd");

constexpr const char *HTTP_CONTENT_LENGTH = "Content-Length";
constexpr const char *HTTP_CONTENT_TYPE = "Content-Type";
constexpr const char *HTTP_USER_AGENT = "User-Agent";

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

    http_parser_init(&parser_, HTTP_BOTH);
    http_parser_set_max_header_size(HttpMaximumHeaderSize);

    parser_.data = this;
}

int32_t HttpRequest::parse(const char *data, size_t length) {
    auto nparsed = http_parser_execute(&parser_, &settings_, data, length);

    if (parser_.http_errno > 0) {
        auto err = (enum http_errno)parser_.http_errno;
        logerror("parser: %s: %s", http_errno_name(err), http_errno_description(err));
        return 0;
    }

    return nparsed;
}

int32_t HttpRequest::on_message_begin() {
    logtrace("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::URL;

    return 0;
}

int32_t HttpRequest::on_url(const char *at, size_t length) {
    logtrace("%s", __PRETTY_FUNCTION__);

    auto n = std::min(length, sizeof(url_));
    strncpy(url_, at, n);
    url_[length] = 0;
    url_parser_ = { url_ };

    state_ = HttpRequestState::Headers;

    return 0;
}

int32_t HttpRequest::on_header_field(const char *at, size_t length) {
    logtrace("%s", __PRETTY_FUNCTION__);

    header_name_ = at;
    header_name_len_ = length;

    return 0;
}

static char *trim(char *s) {
    auto n = strlen(s);
    if (n == 0) {
        return s;
    }
    char *p = s + n - 1;
    while (p >= s && isspace(*p)) {
        *p = '\0';
        p--;
    }
    return s;
}

static WellKnownContentType get_content_type(const char *value) {
    if (strstr(value, "application/fkhttp") != nullptr) {
        return WellKnownContentType::ApplicationFkHttp;
    }
    else if (strstr(value, "text/plain") != nullptr) {
        return WellKnownContentType::TextPlain;
    }
    else if (strstr(value, "application/octet-stream") != nullptr) {
        return WellKnownContentType::ApplicationOctetStream;
    }
    return WellKnownContentType::Unknown;
}

int32_t HttpRequest::on_header_value(const char *at, size_t length) {
    logtrace("%s", __PRETTY_FUNCTION__);

    if (strncasecmp(header_name_, HTTP_CONTENT_LENGTH, header_name_len_) == 0) {
        length_ = atoi(at);
        logtrace("content-length: %" PRIu32, length_);
    }

    if (strncasecmp(header_name_, HTTP_CONTENT_TYPE, header_name_len_) == 0) {
        auto value = trim(pool_->strndup(at, length));
        content_type_ = get_content_type(value);
        logtrace("content-type: %s (%" PRId32 ")", value, (int32_t)content_type_);
    }

    if (strncasecmp(header_name_, HTTP_USER_AGENT, header_name_len_) == 0) {
        auto value = trim(pool_->strndup(at, length));
        user_agent_ = value;
        logtrace("user-agent: %s", value);
    }

    return 0;
}

int32_t HttpRequest::on_headers_complete() {
    logtrace("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::Body;

    return 0;
}

int32_t HttpRequest::on_data(const char *at, size_t length) {
    logtrace("%s(0x%p, %" PRIu32 ")", __PRETTY_FUNCTION__, at, (int32_t)length);

    FK_ASSERT(buffered_body_ == nullptr && buffered_body_length_ == 0);

    buffered_body_ = (uint8_t const *)at;
    buffered_body_length_ = length;

    return 0;
}

int32_t HttpRequest::on_message_complete() {
    logtrace("%s", __PRETTY_FUNCTION__);

    state_ = HttpRequestState::Consumed;

    return 0;
}

int32_t HttpRequest::read_buffered_body(uint8_t *buffer, size_t size) {
    if (buffered_body_ == nullptr) {
        return 0;
    }

    auto returning = std::min(buffered_body_length_, size);
    memcpy(buffer, buffered_body_, returning);

    buffered_body_length_ -= returning;
    if (buffered_body_length_ == 0) {
        buffered_body_ = nullptr;
    }
    else {
        buffered_body_ += returning;
    }

    return returning;
}

}
