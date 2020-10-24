#include "platform.h"
#include "common.h"
#include "networking/dns_message.h"
#include "ethutil.h"

namespace fk {

FK_DECLARE_LOGGER("dns");

DNSMessage::DNSMessage(Pool *pool, uint8_t *buffer, size_t size) : pool_(pool), buffer_(buffer), size_(size),
                                                                   header_((dns_header_t *)buffer_) {
}

DNSMessage::~DNSMessage() {
}

int16_t DNSMessage::read_name(uint8_t *p, uint8_t *name, size_t size) {
    auto position = 0u;
    auto bytes = 0;
    while (p < end_of_packet()) {
        auto part_length = *p;
        if (part_length == 0){
            p++;
            bytes++;
            break;
        }

        if ((part_length & 0xc0) == 0xc0) {
            // auto offset = part_length & (0x3f);
            // printf("compression: %d\n", offset);
            p += 2;
            bytes += 2;
            break;
        }

        if (name != nullptr) {
            if (position > 0) {
                name[position++] = '.';
            }
            memcpy(name + position, p + 1, part_length);
            position += part_length;
        }
        p += part_length + 1;
        bytes += part_length + 1;
    }
    return bytes;
}

DNSMessage::dns_name_t DNSMessage::read_name(uint8_t *p) {
    auto length = read_name(p, nullptr, 0);
    if (length < 0) {
        return { nullptr, length };
    }
    auto name = (uint8_t *)pool_->malloc(length + 1);
    read_name(p, name, length + 1);
    return { (char *)name, length };
}

bool DNSMessage::parse() {
    printf("parsing %zu bytes q=%d a=%d au=%d ad=%d\n", size_,
           number_queries(), number_answers(), number_authorities(), number_additional());

    answers_size();

    return true;
}

uint16_t DNSMessage::read_uint16(pointer_t &pos) {
    auto value = ethutil_ntohs(*(uint16_t *)&pos.p[0]);
    pos.moved += 2;
    pos.p += 2;
    return value;
}

int16_t DNSMessage::queries_size() {
    auto p = after_header();
    auto queries = ethutil_ntohs(header_->number_query);
    auto bytes = 0;
    while (queries > 0) {
        auto name = read_name(p);
        auto record_length = name.length;
        p += record_length;
        // auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        // auto record_class = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;

        printf("  query: %s\n", name.name);

        record_length += 2 * 2;
        bytes += record_length;
        queries--;
    }
    return bytes;
}

int16_t DNSMessage::answers_size() {
    auto p = after_header() + queries_size();
    auto answers = ethutil_ntohs(header_->number_answer);
    auto bytes = 0;
    while (answers > 0) {
        auto name = read_name(p);
        auto record_length = name.length;
        p += record_length;

        if (verbose_) {
            printf("  answer: %s\n", name.name);
        }

        auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        // auto record_class = ethutil_ntohs(*(uint16_t *)&p[2]);
        // auto ttl1 = ethutil_ntohs(*(uint16_t *)&p[4]);
        // auto ttl2 = ethutil_ntohs(*(uint16_t *)&p[6]);
        auto rdlength = ethutil_ntohs(*(uint16_t *)&p[8]);

        p += 5 * 2;

        switch (record_type) {
        case 0x01: {
            if (rdlength != 4) {
                error_ = true;
                return -1;
            }

            if (verbose_) {
                printf("    a record (%d) %d.%d.%d.%d\n", rdlength, p[0], p[1], p[2], p[3]);
            }
            p += rdlength;
            break;
        }
        case 0x1c: { // AAAA Ipv6
            if (verbose_) {
                printf("    aaaa (ipv6) record (%d)\n", rdlength);
            }
            p += rdlength;
            break;
        }
        case 0x10: {
            if (verbose_) {
                printf("    null record (%d)\n", rdlength);
            }
            p += rdlength;
            break;
        }
        case 0x21: { // SRV
            if (verbose_) {
                auto priority = ethutil_ntohs(*(uint16_t *)&p[0]);
                auto weight = ethutil_ntohs(*(uint16_t *)&p[2]);
                auto port = ethutil_ntohs(*(uint16_t *)&p[4]);
                printf("    srv record (%d) pri=%d w=%d port=%d\n", rdlength, priority, weight, port);
            }
            p += rdlength;
            break;
        }
        case 0x0c: {
            auto name = read_name(p);
            if (name.name != nullptr) {
                if (verbose_) {
                    printf("    ptr record (%d) '%s' (%d)\n", rdlength, name.name, name.length);
                }
            }
            p += rdlength;
            break;
        }
        default: {
            if (verbose_) {
                printf("    unknown type (0x%x/%d) (rdlength=%d)\n", record_type, record_type, rdlength);
            }
            p += rdlength;
            break;
        }
        }

        record_length += rdlength + 2 * 5;
        bytes += record_length;
        answers--;
    }

    return bytes;
}

static void write_question(uint8_t *p, const char *name, size_t size) {
    auto name_ptr = name;
    auto memory_end = p + size;
    while (p < memory_end) {
        auto length = 0;
        auto np = strchr((char *)name_ptr, '.');
        if (np == nullptr) {
            length = strlen((char *)name_ptr);
        } else {
            length = np - (char *)name_ptr;
        }
        *p++ = length;
        for (auto i = 0; i < length; ++i) {
            *p++ = *name_ptr++;
        }
        if (np == nullptr) {
            break;
        }
        name_ptr = np + 1;
    }
}

bool DNSMessage::query_service_type(const char *service_type) {
    auto size = 512;
    auto buffer = (uint8_t *)pool_->malloc(size);
    memzero(buffer, size);
    auto position = sizeof(dns_header_t);
    auto header = (dns_header_t *)buffer;

    header->xid = fk_uptime() % 32768;
    header->recursion_desired = 1;

    write_question(buffer + position, "_fk._tcp.local", size - position);

    return true;
}

} // namespace fk
