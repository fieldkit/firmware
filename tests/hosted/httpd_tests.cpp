#include "tests.h"
#include "httpd/httpd.h"
#include <http_parser.h>

#define TOKEN "000000000000000000000000000000000000." \
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000." \
    "0000000-000000000-000000_000000000000000000000000000000000_000000000000000000000000000"

using namespace fk;

class HttpParsingSuite : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

TEST_F(HttpParsingSuite, SimpleGet1) {
    const char *req_header =
        "GET / HTTP/1.1\n"
        "Content-Type: text/plain\n"
        "Content-Length: 0\n"
        "\n";

    HttpRequest req;

    ASSERT_EQ(req.parse(req_header, strlen(req_header)), (int32_t)strlen(req_header));

    ASSERT_STREQ(req.url(), "/");
    ASSERT_EQ(req.length(), (uint32_t)0);
}

TEST_F(HttpParsingSuite, SimpleGet2) {
    char body[32] = { 0 };

    const char *req_header =
        "GET /fk/1/status HTTP/1.1\n"
        "Connection: close\n"
        "Content-Length: 32\n"
        "Content-Type: text/plain\n"
        "Content-Encoding: gzip\n"
        "Authorization: Bearer " TOKEN "\n"
        "User-Agent: curl/7.58.0\n"
        "Accept: */*\n"
        "\n";

    HttpRequest req;

    ASSERT_EQ(req.parse(req_header, strlen(req_header)), (int32_t)strlen(req_header));

    ASSERT_STREQ(req.url(), "/fk/1/status");
    ASSERT_EQ(req.length(), (uint32_t)32);

    ASSERT_EQ(req.parse(body, sizeof(body)), (int32_t)sizeof(body));
}
