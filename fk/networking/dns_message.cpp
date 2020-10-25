#include "networking/dns_message.h"
#include "common.h"
#include "platform.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("dns");

enum class RecordTypes : uint16_t {
    A = 0x01,
    Aaaa = 0x1c,
    Null = 0x10,
    Ptr = 0x0c,
    Srv = 0x21,
};

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
        case (uint16_t)RecordTypes::A: {
            if (rdlength != 4) {
                error_ = true;
                return -1;
            }

            logdebug("a record (%d) %d.%d.%d.%d", rdlength, p[0], p[1], p[2], p[3]);
            p += rdlength;
            break;
        }
        case (uint16_t)RecordTypes::Aaaa: {
            logdebug("aaaa (ipv6) record (%d)", rdlength);
            p += rdlength;
            break;
        }
        case (uint16_t)RecordTypes::Null: {
            logdebug("null record (%d)", rdlength);
            p += rdlength;
            break;
        }
        case (uint16_t)RecordTypes::Srv: {
            auto priority = ethutil_ntohs(*(uint16_t *)&p[0]);
            auto weight = ethutil_ntohs(*(uint16_t *)&p[2]);
            auto port = ethutil_ntohs(*(uint16_t *)&p[4]);
            auto name = read_name(p + 6);
            if (name.name != nullptr) {
                logdebug("srv record (%d) pri=%d w=%d port=%d '%s'", rdlength, priority, weight, port, name.name);
            }
            else {
                logdebug("srv record (%d) pri=%d w=%d port=%d <noname>", rdlength, priority, weight, port);
            }
            p += rdlength;
            break;
        }
        case (uint16_t)RecordTypes::Ptr: {
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

DNSWriter::DNSWriter(Pool *pool) : pool_(pool),
                                   buffer_((uint8_t *)pool->malloc(size_)),
                                   writer_{ nullptr, buffer_, size_ } {
    begin();
}

void DNSWriter::begin() {
    dns_header_t header;
    memzero(&header, sizeof(dns_header_t));
    writer_.write((uint8_t *)&header, sizeof(dns_header_t));
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

int32_t DNSWriter::write_question(const char *name, uint16_t record_type, uint16_t record_class) {
    header()->number_query = ethutil_htons(ethutil_ntohs(header()->number_query) + 1);

    auto name_bytes = write_name(&writer_, name);
    if (name_bytes < 0) {
        return -1;
    }

    if (writer_.write_u16(ethutil_htons(record_type)) < 0) {
        return -1;
    }

    if (writer_.write_u16(ethutil_htons(record_class)) < 0) {
        return -1;
    }

    return name_bytes + 4;
}

EncodedMessage *DNSWriter::query_service_type(uint16_t xid, const char *service_type) {
    header()->xid = xid;
    header()->recursion_desired = 0;

    if (write_question(service_type, 12, 1) < 0) {
        return nullptr;
    }

    return finish();
}

int32_t DNSWriter::write_answer_ptr(const char *name, const char *ptr) {
    header()->number_answer = ethutil_htons(ethutil_ntohs(header()->number_answer) + 1);

    if (write_name(&writer_, name) < 0) {
        return -1;
    }

    auto name_length = write_name(nullptr, ptr);

    if (writer_.write_u16(ethutil_htons((uint16_t)RecordTypes::Ptr)) < 0) { // type
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(1)) < 0) { // class
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(0)) < 0) { // ttl1
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(120)) < 0) { // ttl2
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(name_length)) < 0) { // rdlength
        return -1;
    }

    // rddata

    if (write_name(&writer_, ptr) < 0) {
        return -1;
    }

    return 0;
}

int32_t DNSWriter::write_answer_srv(const char *name, const char *server_name, uint16_t port) {
    header()->number_answer = ethutil_htons(ethutil_ntohs(header()->number_answer) + 1);

    if (write_name(&writer_, name) < 0) {
        return -1;
    }

    auto server_name_length = write_name(nullptr, server_name);

    if (writer_.write_u16(ethutil_htons((uint16_t)RecordTypes::Srv)) < 0) { // type
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(1)) < 0) { // class
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(0)) < 0) { // ttl1
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(120)) < 0) { // ttl2
        return -1;
    }
    if (writer_.write_u16(ethutil_htons(2 + 2 + 2 + server_name_length)) < 0) { // rdlength
        return -1;
    }

    // rddata

    writer_.write_u16(ethutil_htons(0));
    writer_.write_u16(ethutil_htons(0));
    writer_.write_u16(ethutil_htons(port));

    if (write_name(&writer_, server_name) < 0) {
        return -1;
    }

    return 0;
}

EncodedMessage *DNSWriter::answer_service_type(uint16_t xid, const char *service_type, const char *name) {
    auto fq_name = pool_->sprintf("%s.%s", name, service_type);
    auto server_name = pool_->sprintf("%s.%s", name, "local");

    header()->xid = xid;
    header()->recursion_desired = 0;

    if (write_question(service_type, 12, 1) < 0) {
        return nullptr;
    }

    if (write_answer_srv(fq_name, server_name, 80) < 0) {
        return nullptr;
    }

    if (write_answer_ptr(service_type, fq_name) < 0) {
        return nullptr;
    }

    return finish();
}

EncodedMessage *DNSWriter::finish() {
    return pool_->wrap(buffer_, writer_.position());
}

} // namespace fk
