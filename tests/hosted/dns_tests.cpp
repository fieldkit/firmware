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
        log_configure_level(LogLevels::DEBUG);
    }

    void TearDown() override {
    }

};

TEST_F(DnsSuite, ParseAll) {
    for (auto &p : packets) {
        StandardPool pool{ "dns" };
        auto buffer = (uint8_t *)pool.copy(p.data, p.size);
        DNSMessage message{ &pool, buffer, p.size };
        if (p.valid) {
            ASSERT_GE(message.parse(), 0);
        }
        else {
            ASSERT_LT(message.parse(), 0);
        }
    }
}

TEST_F(DnsSuite, BuildQueryServiceType) {
    StandardPool pool{ "dns" };
    DNSWriter writer{ &pool };
    auto encoded = writer.query_service_type(0, "_fk._http._tcp.local");
    ASSERT_EQ(encoded->size, sizeof(packet_16));
    ASSERT_EQ(encoded->size, 38u);
    ASSERT_TRUE(0 == std::memcmp(encoded->buffer, packet_16, encoded->size));

    DNSMessage message{ &pool, encoded->buffer, encoded->size };
    message.parse();
}

TEST_F(DnsSuite, BuildAnswerService) {
    StandardPool pool{ "dns" };
    DNSWriter writer{ &pool };
    auto encoded = writer.answer_service_type(0, "_fk._tcp.local", "4fb1a1555336573232202020ff193811");

    DNSMessage message{ &pool, encoded->buffer, encoded->size };
    message.parse();

    // ASSERT_EQ(encoded->size, 173u);
}
