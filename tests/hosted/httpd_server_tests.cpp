#include "tests.h"
#include "httpd/httpd.h"

#include "mocks_and_fakes.h"

using testing::Return;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::_;
using namespace fk;

class HttpServerSuite : public ::testing::Test {
protected:
};

TEST_F(HttpServerSuite, BeginSuccessful) {
    MockNetwork network;
    HttpServer server{ &network };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillOnce(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(true));

    ASSERT_TRUE(server.begin());
}

static NetworkStatus flow_time_and_return_disconnected() {
    return NetworkStatus::Ready;
}

TEST_F(HttpServerSuite, BeginTakesTooLong) {
    MockNetwork network;
    HttpServer server{ &network };

    std::vector<uint32_t> times = { 0, 1000, 5000, 31000 };
    fk_fake_uptime(times);

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(InvokeWithoutArgs(flow_time_and_return_disconnected));

    ASSERT_FALSE(server.begin());
}
