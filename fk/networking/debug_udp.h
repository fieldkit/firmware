#pragma once

#include <Udp.h>
#include "common.h"
#include "pool.h"

namespace fk {

class DebugUDP : public UDP {
private:
    UDP *target_{ nullptr };
    const char *name_{ nullptr };
    Pool *pool_{ nullptr };
    Pool *dns_pool_{ nullptr };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };
    size_t available_{ 0 };
    uint32_t throttled_{ 0 };

public:
    DebugUDP(UDP &target, const char *name);
    virtual ~DebugUDP();

public:
    void pool(Pool *pool);
    void dns_pool(Pool *pool);

public:
    uint8_t begin(uint16_t port) override {
        return target_->begin(port);
    }
    uint8_t beginMulticast(IPAddress address, uint16_t port) override {
        return target_->beginMulticast(address, port);
    }
    int parsePacket() override;
    int beginPacket(IPAddress ip, uint16_t port) override;
    int beginPacket(const char *host, uint16_t port) override;
    int endPacket() override;
    size_t write(uint8_t) override;
    size_t write(uint8_t const *buffer, size_t size) override;
    int read(unsigned char *buffer, size_t len) override;
    int read(char *buffer, size_t len) override {
        return read((unsigned char *)buffer, len);
    };
    void flush() override {
        target_->flush();
    }
    int available() override;
    int read() override {
        FK_ASSERT(false);
        return target_->read();
    }
    int peek() override {
        return target_->peek();
    }
    IPAddress remoteIP() override {
        return target_->remoteIP();
    }
    uint16_t remotePort() override {
        return target_->remotePort();
    }
    void stop() {
        target_->stop();
    }

private:
    size_t append(uint8_t const *buffer, size_t size);
    size_t debug(char const *prefix, uint8_t const *buffer, size_t size);

private:
    void reply_with_query(const char *name);

};

} // namespace fk
