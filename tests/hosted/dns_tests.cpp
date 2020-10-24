#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "networking/dns_message.h"
#include "dns_packets.h"

using namespace fk;

FK_DECLARE_LOGGER("dns");

class DnsSuite : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

};

TEST_F(DnsSuite, ParseAll) {
    for (auto &p : packets) {
        StandardPool pool{ "dns" };
        auto buffer = (uint8_t *)pool.copy(p.data, p.size);
        DNSMessage message{ &pool, buffer, p.size };

        printf("\n--------------------\n\n");
        message.parse();
    }
}

TEST_F(DnsSuite, BuildQueryServiceType) {
    StandardPool pool{ "dns" };
    DNSMessage message{ &pool };
    message.query_service_type("_fk._http._tcp");
}

TEST_F(DnsSuite, BuildAnswerService) {
    StandardPool pool{ "dns" };
    DNSMessage message{ &pool };
    message.query_service_type("_fk._http._tcp");
}
