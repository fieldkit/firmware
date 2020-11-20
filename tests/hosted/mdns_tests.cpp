#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "networking/debug_udp.h"
#include "networking/dns_message.h"
#include "hal/metal/mdns_pool_allocator.h"
#include "dns_packets.h"

#include <ArduinoMDNS.h>

using namespace fk;

FK_DECLARE_LOGGER("mdns");

class MdnsSuite : public ::testing::Test {
protected:
    void SetUp() override {
        // log_configure_level(LogLevels::DEBUG);
    }

    void TearDown() override {
    }

};

class DummyUDP : public UDP {
private:
    packet_t const *queued_{ nullptr };

public:
    void queue(packet_t const *p) {
        queued_ = p;
    }

public:
    uint8_t begin(uint16_t) override {
        return true;
    }

    uint8_t beginMulticast(IPAddress, uint16_t) override {
        return true;
    }

    void stop() override {
    }

    int beginPacket(const char *host, uint16_t port) {
        return 0;
    }

    int beginPacket(IPAddress ip, uint16_t port) override {
        return 0;
    }

    int endPacket() override {
        return 0;
    }

    int read(char *buffer, size_t len) override {
        return 0;
    }

    size_t write(const uint8_t *buffer, size_t size) override {
        return 0;
    }

    size_t write(uint8_t) override {
        return 0;
    }

    int parsePacket() override {
        if (queued_ == nullptr) {
            return 0;
        }
        loginfo("dummy-udp: parse-packet");
        return queued_->size;
    }

    int read(unsigned char *buffer, size_t len) override {
        if (queued_ == nullptr) {
            return 0;
        }
        FK_ASSERT(len == queued_->size); // Make our lives easier in
                                         // here, maybe.
        loginfo("dummy-udp:read %zu", len);
        memcpy(buffer, queued_->data, len);
        queued_ = nullptr;
        return len;
    }

    int read() override {
        FK_ASSERT(false);
        return 0;
    }

    int peek() override {
        FK_ASSERT(false);
        return 0;
    }

    int available() override {
        return 0;
    }

    void flush() override {
    }

    IPAddress remoteIP() override {
        return { };
    }

    uint16_t remotePort() override {
        return 0;
    }
};

TEST_F(MdnsSuite, MDNS) {
    StandardPool pool{ "dns" };
    MDNSPoolAllocator allocator{ &pool };
    DummyUDP udp;
    DebugUDP debug_udp{ udp, "debug" };
    MDNS mdns(debug_udp);
    mdns.allocator(&allocator);

    ASSERT_TRUE(mdns.begin({}));

    ASSERT_TRUE(mdns.addServiceRecord("_fk._tcp", 80, MDNSServiceTCP));

    for (auto &p : packets) {
        udp.queue(&p);
        mdns.run();
    }
}
