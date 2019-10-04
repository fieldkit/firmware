#include <fk-app-protocol.h>

#include "tests.h"
#include "networking/networking.h"

#include <http_parser.h>

#define TOKEN "000000000000000000000000000000000000." \
    "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000." \
    "0000000-000000000-000000_000000000000000000000000000000000_000000000000000000000000000"

using namespace fk;

class HttpBasicParsingSuite : public ::testing::Test {
protected:
    MallocPool pool_{ "tests", 1024 * 100 };
};

TEST_F(HttpBasicParsingSuite, SimpleGet1) {
    const char *req_header =
        "GET / HTTP/1.1\n"
        "Content-Type: text/plain\n"
        "Content-Length: 0\n"
        "\n";

    HttpRequest req{ &pool_ };

    ASSERT_EQ(req.parse(req_header, strlen(req_header)), 0);

    ASSERT_STREQ(req.url(), "/");
    ASSERT_EQ(req.length(), (uint32_t)0);
}

TEST_F(HttpBasicParsingSuite, SimpleGet2) {
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

    HttpRequest req{ &pool_ };

    ASSERT_EQ(req.parse(req_header, strlen(req_header)), 0);

    ASSERT_STREQ(req.url(), "/fk/1/status");
    ASSERT_EQ(req.length(), (uint32_t)32);

    ASSERT_EQ(req.parse(body, sizeof(body)), 0);
}

class HttpRoutingSuite : public ::testing::Test {
protected:
    void SetUp() override {
    }
};

class DummyHandler : public HttpHandler {
public:
    bool handle(Connection *connection, Pool &pool) override {
        return true;
    }

};

TEST_F(HttpRoutingSuite, WhenEmpty) {
    HttpRouter router;

    auto handler = router.route("/fk/v1");

    ASSERT_EQ(handler, nullptr);
}

TEST_F(HttpRoutingSuite, WithASingleBasicRoute) {
    HttpRouter router;
    DummyHandler h1;
    HttpRoute r1{ "/fk/v1", &h1 };

    router.add_route(&r1);

    ASSERT_EQ(router.route("/invalid-route"), nullptr);
    ASSERT_EQ(router.route("/fk/v1"), &h1);
}

TEST_F(HttpRoutingSuite, WithMultipleBasicRoutes) {
    HttpRouter router;
    DummyHandler handlers[4];
    HttpRoute routes[4]{
      { "/fk/v1", &handlers[0] },
      { "/fk/v2", &handlers[1] },
      { "/fk/v3", &handlers[2] },
      { "/fk/v4", &handlers[3] }
    };

    for (auto i = (size_t)0; i < 4; ++i) {
        router.add_route(&routes[i]);
    }

    ASSERT_EQ(router.route("/invalid-route"), nullptr);
    ASSERT_EQ(router.route("/fk/v3"), &handlers[2]);
    ASSERT_EQ(router.route("/fk/v4"), &handlers[3]);
    ASSERT_EQ(router.route("/fk/v2"), &handlers[1]);
    ASSERT_EQ(router.route("/fk/v1"), &handlers[0]);
}

class HttpParsingQuerySuite : public ::testing::Test {
protected:
    MallocPool pool_{ "tests", 1024 * 100 };
};

TEST_F(HttpParsingQuerySuite, SimpleGet1) {
    fk_app_WireMessageQuery query = fk_app_WireMessageQuery_init_default;
    query.type = fk_app_QueryType_QUERY_STATUS;

    auto encoded = pool_.encode(fk_app_WireMessageQuery_fields, &query);

    auto req_header = pool_.sprintf(
        "GET / HTTP/1.1\n"
        "Content-Type: text/plain\n"
        "Content-Length: %d\n"
        "\n", encoded->size);

    HttpRequest req{ &pool_ };

    ASSERT_EQ(req.parse(req_header, strlen(req_header)), 0);
    ASSERT_EQ(req.parse((const char *)encoded->buffer, encoded->size), 0);

    ASSERT_STREQ(req.url(), "/");
    ASSERT_EQ(req.length(), (uint32_t)3);
}
