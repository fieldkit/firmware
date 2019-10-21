#include "tests.h"
#include "networking/networking.h"
#include "networking/network_task.h"

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
    MallocPool pool{ "pool", 256 };
    HttpServer server{ &network };
    NetworkTask task{ &network, server };
    GlobalState gs;
    bzero(&gs, sizeof(GlobalState));
    gs.network.config.wifi_networks[0] = WifiNetworkInfo{ "Conservify", "blahblah" };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(true));

    ASSERT_TRUE(task.begin({ }, 0, pool));
    ASSERT_TRUE(server.serve());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillRepeatedly(Return(true));
}

TEST_F(HttpServerSuite, WhenBeginTakesTooLong) {
    configuration_t fkc;
    MockNetwork network;
    MallocPool pool{ "pool", 256 };
    HttpServer server{ &network };
    NetworkTask task{ &network, server };
    GlobalState gs;
    bzero(&gs, sizeof(GlobalState));
    gs.network.config.wifi_networks[0] = WifiNetworkInfo{ "Conservify", "blahblah" };

    fk_fake_uptime({ 0, 1000, 5000, 31000, 36000, 61000 });

    EXPECT_CALL(network, begin(_)).WillOnce(Return(false));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Ready));

    ASSERT_FALSE(task.begin({ }, 0, pool));

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillRepeatedly(Return(true));
}

TEST_F(HttpServerSuite, WhenServeFails) {
    configuration_t fkc;
    MockNetwork network;
    MallocPool pool{ "pool", 256 };
    HttpServer server{ &network };
    NetworkTask task{ &network, server };
    GlobalState gs;
    bzero(&gs, sizeof(GlobalState));
    gs.network.config.wifi_networks[0] = WifiNetworkInfo{ "Conservify", "blahblah" };

    EXPECT_CALL(network, begin(_)).WillOnce(Return(true));
    EXPECT_CALL(network, status()).WillRepeatedly(Return(NetworkStatus::Connected));
    EXPECT_CALL(network, serve()).WillOnce(Return(false));

    ASSERT_TRUE(task.begin({ }, 0, pool));
    ASSERT_FALSE(server.serve());

    // Called in the dtor.
    EXPECT_CALL(network, stop()).WillRepeatedly(Return(true));
}
