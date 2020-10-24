#pragma once

#if defined(__SAMD51__)

#include <WiFi101.h>
#include <WiFiUdp.h>

#undef min
#undef max
#undef abs

#include "common.h"
#include "pool.h"

namespace fk {

class DebugUDP : public WiFiUDP {
private:
    const char *name_{ nullptr };
    Pool *pool_{ nullptr };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };
    size_t expected_{ 0 };

public:
    DebugUDP(const char *name);
    virtual ~DebugUDP();

public:
    void pool(Pool *pool);

public:
    int parsePacket() override;
    int beginPacket(IPAddress ip, uint16_t port) override;
    int beginPacket(const char *host, uint16_t port) override;
    int endPacket() override;
    size_t write(uint8_t) override;
    size_t write(uint8_t const *buffer, size_t size) override;
    int read(unsigned char *buffer, size_t len);
    int read(char *buffer, size_t len) {
        return read((unsigned char *)buffer, len);
    };

private:
    size_t append(uint8_t const *buffer, size_t size);
    size_t debug(char const *prefix, uint8_t const *buffer, size_t size);

};

} // namespace fk

#endif // defined(__SAMD51__)
