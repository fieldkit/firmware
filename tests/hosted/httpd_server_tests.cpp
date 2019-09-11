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
    configuration_t fkc;
    MockNetwork network;
    HttpServer server{ &network, &fkc };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(true));

    ASSERT_TRUE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}

TEST_F(HttpServerSuite, WhenBeginTakesTooLong) {
    configuration_t fkc;
    MockNetwork network;
    HttpServer server{ &network, &fkc };

    fk_fake_uptime({ 0, 1000, 5000, 31000, 36000, 61000 });

    EXPECT_CALL(network, begin(_))
        .WillOnce(Return(true))
        .WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Ready));

    ASSERT_FALSE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}

TEST_F(HttpServerSuite, WhenServeFails) {
    configuration_t fkc;
    MockNetwork network;
    HttpServer server{ &network, &fkc };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(false));

    ASSERT_FALSE(server.begin());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillOnce(Return(true));
}
