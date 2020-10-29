#include "platform.h"
#include "hal/metal/debug_udp.h"
#include "common.h"
#include "utilities.h"
#include "networking/dns_message.h"
#include "config.h"

#if defined(__SAMD51__)

namespace fk {

constexpr size_t NetworkDebugUDPMaximumPacketSize = 1024;

FK_DECLARE_LOGGER("dbgudp");

DebugUDP::DebugUDP(const char *name) : name_(name) {
}

DebugUDP::~DebugUDP() {
}

void DebugUDP::pool(Pool *pool) {
    FK_ASSERT(pool != nullptr);

    pool_ = pool;
    size_ = NetworkDebugUDPMaximumPacketSize;
    position_ = 0;
    buffer_ = (uint8_t *)pool_->malloc(size_);
}

void DebugUDP::dns_pool(Pool *pool) {
    dns_pool_ = pool;
}

int DebugUDP::parsePacket() {
    position_ = 0;
    size_ = 0;

    auto parsed_size = WiFiUDP::parsePacket();
    if (parsed_size == 0) {
        return 0;
    }

    if (buffer_ == nullptr) {
        return parsed_size;
    }

    // Read the whole packet in and read from our buffer.
    if (parsed_size < (int32_t)NetworkDebugUDPMaximumPacketSize) {
        auto bytes_read = WiFiUDP::read(buffer_, parsed_size);
        if (bytes_read != parsed_size) {
            logwarn("read less than available (%d vs %d)", bytes_read, parsed_size);
        }
        size_ = bytes_read;

        debug("udp-recv", buffer_, bytes_read);

        if (NetworkDebugDnsParsing) {
            if (dns_pool_ != nullptr && size_ > 0) {
                DNSReader message{ dns_pool_, buffer_, size_ };
                if (message.parse()) {
                    for (auto name : message.names()) {
                        auto found_suffix = strstr(name, "_fk._tcp.local");
                        if (found_suffix != nullptr && found_suffix != name) {
                            loginfo("query for: %s", name);
                        }
                    }
                }
            }
        }
    }

    return parsed_size;
}

int DebugUDP::beginPacket(IPAddress ip, uint16_t port) {
    position_ = 0;
    return WiFiUDP::beginPacket(ip, port);
}

int DebugUDP::beginPacket(const char *host, uint16_t port) {
    position_ = 0;
    return WiFiUDP::beginPacket(host, port);
}

int DebugUDP::endPacket() {
    if (buffer_ != nullptr && position_ > 0) {
        debug("udp-send", buffer_, position_);

        if (NetworkDebugDnsParsing) {
            if (dns_pool_ != nullptr) {
                DNSReader message{ dns_pool_, buffer_, position_ };
                message.parse();
            }
        }

        position_ = 0;
    }
    return WiFiUDP::endPacket();
}

int DebugUDP::read(unsigned char *buffer, size_t len) {
    if (size_ > 0) {
        auto remaining = size_ - position_;
        if (remaining < 0) {
            return 0;
        }
        auto copying = std::min(remaining, len);
        memcpy(buffer, buffer_ + position_, copying);
        position_ += len;
        return copying;
    }
    return WiFiUDP::read(buffer, len);
}

size_t DebugUDP::write(uint8_t c) {
    append(&c, 1);
    return WiFiUDP::write(c);
}

size_t DebugUDP::write(uint8_t const *buffer, size_t size) {
    append(buffer, size);
    return WiFiUDP::write(buffer, size);
}

size_t DebugUDP::append(uint8_t const *buffer, size_t size) {
    if (buffer_ != nullptr) {
        for (auto i = 0u; i < size; ++i) {
            if (position_ == NetworkDebugUDPMaximumPacketSize) {
                break;
            }
            buffer_[position_] = buffer[i];
            position_++;
        }
    }
    return size;
}

size_t DebugUDP::debug(char const *prefix, uint8_t const *buffer, size_t size) {
    fk_logs_dump_memory("%s[%s]: ", buffer, size, prefix, name_);
    return size;
}

} // namespace fk

#endif
