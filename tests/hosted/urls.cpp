#include "tests.h"
#include "worker.h"
#include "networking/url_parser.h"

using namespace fk;

class UrlParsingSuite : public ::testing::Test {
protected:

};

TEST_F(UrlParsingSuite, Basic) {
    StaticPool<1024> pool("Pool");

    UrlParser p1{ pool.strdup("http://www.fkdev.org") };
    ASSERT_STREQ(p1.server(), "www.fkdev.org");
    ASSERT_EQ(p1.port(), 80);

    UrlParser p2{ pool.strdup("http://www.fkdev.org/fk/v1") };
    ASSERT_STREQ(p2.server(), "www.fkdev.org");
    ASSERT_EQ(p2.port(), 80);

    UrlParser p3{ pool.strdup("http://www.fkdev.org:2380/fk/v1") };
    ASSERT_STREQ(p3.server(), "www.fkdev.org");
    ASSERT_EQ(p3.port(), 2380);

    UrlParser p4{ pool.strdup("https://www.fkdev.org:2380/fk/v1") };
    ASSERT_STREQ(p4.server(), "www.fkdev.org");
    ASSERT_EQ(p4.port(), 2380);
    ASSERT_EQ(p4.query_string(), nullptr);

    UrlParser p5{ pool.strdup("/fk/v1/download") };
    ASSERT_EQ(p5.server(), nullptr);
    ASSERT_STREQ(p5.path(), "/fk/v1/download");
    ASSERT_EQ(p5.query_string(), nullptr);

    UrlParser p6{ pool.strdup("/fk/v1/download/meta?first=100") };
    ASSERT_EQ(p6.server(), nullptr);
    ASSERT_STREQ(p6.path(), "/fk/v1/download/meta");
    ASSERT_STREQ(p6.query_string(), "first=100");
    ASSERT_STREQ(p6.find_query_param("first", pool), "100");
    ASSERT_EQ(p6.find_query_param("last", pool), nullptr);

    UrlParser p7{ pool.strdup("/fk/v1/download/meta?first=100&last=200") };
    ASSERT_EQ(p7.server(), nullptr);
    ASSERT_STREQ(p7.path(), "/fk/v1/download/meta");
    ASSERT_STREQ(p7.query_string(), "first=100&last=200");
    ASSERT_STREQ(p7.find_query_param("first", pool), "100");
    ASSERT_STREQ(p7.find_query_param("last", pool), "200");

    UrlParser p8{ pool.strdup("/fk/v1/download/meta?first=100&last=200") };
    ASSERT_EQ(p8.server(), nullptr);
    ASSERT_STREQ(p8.path(), "/fk/v1/download/meta");
    ASSERT_STREQ(p8.query_string(), "first=100&last=200");
    ASSERT_STREQ(p8.find_query_param("last", pool), "200");
    ASSERT_STREQ(p8.find_query_param("first", pool), "100");
}
