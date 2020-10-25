#include "networking/dns_message.h"
#include "common.h"
#include "platform.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("dns");

DNSMessage::DNSMessage(Pool *pool, uint8_t *buffer, size_t size)
    : pool_(pool), buffer_(buffer), size_(size), header_((dns_header_t *)buffer_) {
}

DNSMessage::~DNSMessage() {
}

DNSMessage::dns_name_length_t DNSMessage::read_name(uint8_t *p, uint8_t *name, size_t size) {
    auto position = 0u;
    auto length = 0u;
    auto bytes = 0u;
    while (p < end_of_packet()) {
        // Check length, 0 means we're done reading this name.
        auto part_length = *p;
        if (part_length == 0) {
            p++;
            bytes++;
            break;
        }

        // Check for DNS compression. I'm so confused by the
        // specifications on this one. It seems like offset should
        // occupy the remaining part of the byte, only I keep seeing
        // the pointer offset in the byte after? What's going on?
        if ((part_length & 0xc0) == 0xc0) {
            if (position >  0) {
                if (name != nullptr) {
                    name[position++] = '.';
                }
                length++;
            }

            auto offset = p[1];
            auto pointer_bytes = read_name(buffer_ + offset, name + position, size - position);
            if (pointer_bytes.compressed < 0) {
                return { -1, -1 };
            }
            p += 2;
            bytes += 2;
            length += pointer_bytes.name;
            break;
        }

        // Are we doing any copying? We get called once to calculate sizes.
        if (name != nullptr) {
            if (position > 0) {
                name[position++] = '.';
                length++;
            }
            memcpy(name + position, p + 1, part_length);
            position += part_length;
        }

        // Move our pointer and the number of bytes.
        p += part_length + 1;
        bytes += part_length + 1;
        length += part_length;
    }
    if (p >= end_of_packet() + 1) {
        return { -1, -1 };
    }
    return { (int16_t)bytes, (int16_t)length };
}

DNSMessage::dns_name_t DNSMessage::read_name(uint8_t *p) {
    // Read the name once to get the length and then we can allocate
    // memory for the name.
    auto lengths = read_name(p, nullptr, 0);
    if (lengths.compressed < 0) {
        return { nullptr, -1 };
    }
    auto name = (uint8_t *)pool_->malloc(lengths.name + 1);
    read_name(p, name, lengths.name + 1);
    return { (char *)name, lengths.compressed };
}

int16_t DNSMessage::parse() {
    loginfo("parsing %zu bytes q=%d a=%d au=%d ad=%d", size_, number_queries(), number_answers(),
            number_authorities(), number_additional());

    return answers_size();
}

uint16_t DNSMessage::read_uint16(pointer_t &pos) {
    auto value = ethutil_ntohs(*(uint16_t *)&pos.p[0]);
    pos.moved += 2;
    pos.p += 2;
    return value;
}

int16_t DNSMessage::queries_size() {
    auto queries = ethutil_ntohs(header_->number_query);
    auto bytes = 0;
    auto p = after_header();
    while (queries > 0 && p < end_of_packet()) {
        auto name = read_name(p);
        auto record_length = name.length;
        p += record_length;

        auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        auto record_class = ethutil_ntohs(*(uint16_t *)&p[2]);

        loginfo("query: '%s' type=%d class=%d", name.name, record_type, record_class);

        record_length += 2 * 2;
        bytes += record_length;
        queries--;
    }
    return bytes;
}

int16_t DNSMessage::answers_size() {
    auto answers = ethutil_ntohs(header_->number_answer);
    auto bytes = 0;
    auto p = after_header() + queries_size();
    while (answers > 0 && p < end_of_packet()) {
        auto name = read_name(p);
        auto record_length = name.length;
        p += record_length;

        // Technically possible if rdlength is 0.
        if (p + 5 * 2 >= end_of_packet() + 1) {
            return -1;
        }

        auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        auto record_class = ethutil_ntohs(*(uint16_t *)&p[2]);
        auto ttl1 = ethutil_ntohs(*(uint16_t *)&p[4]);
        auto ttl2 = ethutil_ntohs(*(uint16_t *)&p[6]);
        auto rdlength = ethutil_ntohs(*(uint16_t *)&p[8]);

        p += 5 * 2;

        logdebug("answer: '%s' type=%d class=%d ttl=%d/%d", name.name, record_type, record_class, ttl1, ttl2);

        if (p + rdlength >= end_of_packet() + 1) {
            logwarn("answers: packet overflow");
            return -1;
        }

        switch (record_type) {
        case 0x01: { // A
            if (rdlength != 4) {
                error_ = true;
                return -1;
            }

            logdebug("a record (%d) %d.%d.%d.%d", rdlength, p[0], p[1], p[2], p[3]);
            p += rdlength;
            break;
        }
        case 0x1c: { // AAAA Ipv6
            logdebug("aaaa (ipv6) record (%d)", rdlength);
            p += rdlength;
            break;
        }
        case 0x10: { // NULL
            logdebug("null record (%d)", rdlength);
            p += rdlength;
            break;
        }
        case 0x21: { // SRV
            auto priority = ethutil_ntohs(*(uint16_t *)&p[0]);
            auto weight = ethutil_ntohs(*(uint16_t *)&p[2]);
            auto port = ethutil_ntohs(*(uint16_t *)&p[4]);
            logdebug("srv record (%d) pri=%d w=%d port=%d", rdlength, priority, weight, port);
            p += rdlength;
            break;
        }
        case 0x0c: { // PTR
            auto name = read_name(p);
            if (name.name != nullptr) {
                logdebug("ptr record (%d) '%s' (%d)", rdlength, name.name, name.length);
            }
            else {
                logdebug("ptr record (%d) <noname>", rdlength);
            }
            p += rdlength;
            break;
        }
        default: {
            logdebug("unknown type (0x%x/%d) (rdlength=%d)", record_type, record_type, rdlength);
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

static int32_t write_name(Writer *writer, const char *name) {
    auto name_ptr = name;
    auto bytes = 0u;
    while (true) {
        auto length = 0;
        auto np = strchr((char *)name_ptr, '.');
        if (np == nullptr) {
            length = strlen((char *)name_ptr);
        } else {
            length = np - (char *)name_ptr;
        }
        if (writer != nullptr) {
            if (writer->write_u8((uint8_t)length) < 0) {
                return -1;
            }
            if (writer->write((uint8_t *)name_ptr, length) < 0) {
                return -1;
            }
        }
        bytes += length + 1;
        if (np == nullptr) {
            if (writer != nullptr) {
                if (writer->write_u8(0) < 0) {
                    return -1;
                }
            }
            bytes++;
            break;
        }
        name_ptr = np + 1;
    }
    return bytes;
}

static int32_t write_question(Writer *writer, const char *name, uint16_t record_type, uint16_t record_class) {
    auto name_bytes = write_name(writer, name);
    if (name_bytes < 0) {
        return -1;
    }

    if (writer->write_u16(ethutil_htons(record_type)) < 0) {
        return -1;
    }

    if (writer->write_u16(ethutil_htons(record_class)) < 0) {
        return -1;
    }

    return name_bytes + 4;
}

EncodedMessage *DNSMessage::query_service_type(uint16_t xid, const char *service_type, Pool *pool) {
    auto size = 512u;
    auto buffer = (uint8_t *)pool->malloc(size);
    BufferedWriter writer{ nullptr, buffer, size };
    memzero(buffer, size);

    dns_header_t header;
    header.xid = xid;
    header.recursion_desired = 0;
    header.number_query = ethutil_htons(1);

    if (writer.write((uint8_t *)&header, sizeof(dns_header_t)) < 0) {
        return nullptr;
    }

    if (write_question(&writer, service_type, 12, 1) < 0) {
        return nullptr;
    }

    return pool->wrap(buffer, writer.position());
}

static int32_t write_answer_ptr(Writer *writer, const char *name, const char *ptr) {
    if (write_name(writer, name) < 0) {
        return -1;
    }

    auto name_size = write_name(nullptr, ptr);

    if (writer->write_u16(ethutil_htons(12)) < 0) { // type
        return -1;
    }
    if (writer->write_u16(ethutil_htons(1)) < 0) { // class
        return -1;
    }
    if (writer->write_u16(ethutil_htons(0)) < 0) { // ttl1
        return -1;
    }
    if (writer->write_u16(ethutil_htons(120)) < 0) { // ttl2
        return -1;
    }
    if (writer->write_u16(ethutil_htons(name_size)) < 0) { // rdlength
        return -1;
    }

    // rddata

    if (write_name(writer, ptr) < 0) {
        return -1;
    }

    return 0;
}

static int32_t write_answer_srv(Writer *writer, const char *name, uint16_t port) {
    return 0;
}

EncodedMessage *DNSMessage::answer_service_type(uint16_t xid, const char *service_type, const char *name, Pool *pool) {
    auto size = 512u;
    auto buffer = (uint8_t *)pool->malloc(size);
    BufferedWriter writer{ nullptr, buffer, size };
    memzero(buffer, size);

    dns_header_t header;
    header.xid = xid;
    header.recursion_desired = 0;
    header.number_query = ethutil_htons(1);
    header.number_answer = ethutil_htons(1);

    if (writer.write((uint8_t *)&header, sizeof(dns_header_t)) < 0) {
        return nullptr;
    }

    if (write_question(&writer, service_type, 12, 1) < 0) {
        return nullptr;
    }

    if (write_answer_srv(&writer, service_type, 80) < 0) {
        return nullptr;
    }

    if (write_answer_ptr(&writer, service_type, name) < 0) {
        return nullptr;
    }

    return pool->wrap(buffer, writer.position());
}

} // namespace fk
