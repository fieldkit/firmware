#include "tests.h"
#include "networking/networking.h"

#include "mocks_and_fakes.h"

using testing::Return;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::_;
using namespace fk;

class HttpServerSuite : public ::testing::Test {
};

TEST_F(HttpServerSuite, WhenThingsAllWork) {
    MockNetwork network;
    HttpServer server{ &network };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillOnce(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(true));

    ASSERT_TRUE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}

TEST_F(HttpServerSuite, WhenBeginTakesTooLong) {
    MockNetwork network;
    HttpServer server{ &network };

    fk_fake_uptime({ 0, 1000, 5000, 31000 });

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Ready));

    ASSERT_FALSE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}

TEST_F(HttpServerSuite, WhenServeFails) {
    MockNetwork network;
    HttpServer server{ &network };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillOnce(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(false));

    ASSERT_FALSE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}
