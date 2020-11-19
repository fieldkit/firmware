#include "tests.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "networking/dns_message.h"
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
public:
    uint8_t beginMulticast(IPAddress, uint16_t) {
        return true;
    }

    void stop() {
    }

    int beginPacket(IPAddress ip, uint16_t port) {
        return 0;
    }

    int endPacket() {
        return 0;
    }

    size_t write(const uint8_t *buffer, size_t size) {
        return 0;
    }

    int parsePacket() {
        loginfo("dummy-udp: parse-packet");
        return sizeof(packet_40);
    }

    int read(unsigned char *buffer, size_t len) {
        loginfo("dummy-udp:read %zu", len);
        memcpy(buffer, packet_40, len);
        return len;
    }

    void flush() {
    }

    IPAddress remoteIP() {
        return { };
    }

    uint16_t remotePort() {
        return 0;
    }
};

class MDNSPoolAllocator : public MDNSAllocator {
private:
    Pool *pool_;

public:
    explicit MDNSPoolAllocator(Pool *pool) : pool_(pool) {
    }

public:
    void *malloc(size_t size) override {
        return pool_->calloc(size);
    }

    void free(void *ptr) override {
    }
};

TEST_F(MdnsSuite, DISABLED_MDNS) {
    StandardPool pool{ "dns" };
    MDNSPoolAllocator allocator{ &pool };
    DummyUDP udp;
    MDNS mdns(udp);
    mdns.allocator(&allocator);

    ASSERT_TRUE(mdns.begin({ }));

    ASSERT_TRUE(mdns.addServiceRecord("_fk._tcp", 80, MDNSServiceTCP));

    mdns.run();
}
