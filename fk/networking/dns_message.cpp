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

DNSReader::DNSReader(Pool *pool, uint8_t *buffer, size_t size)
    : pool_(pool), buffer_(buffer), size_(size), reader_{ nullptr, buffer, size, size } {
}

DNSReader::~DNSReader() {
}

DNSReader::dns_name_length_t DNSReader::read_name(BufferedReader *reader, uint8_t *name) {
    auto position = 0u;
    auto length = 0u;
    auto bytes = 0u;
    while (bytes < 255) { // HACK
        if (reader->available() == 0) {
            logwarn("unexpected end");
            return { -1, -1 };
        }

        // Check length, 0 means we're done reading this name.
        auto part_length = reader->read_u8();
        bytes++;

        if (part_length == 0) {
            break;
        }

        // Check for DNS compression. I'm so confused by the
        // specifications on this one. It seems like offset should
        // occupy the remaining part of the byte, only I keep seeing
        // the pointer offset in the byte after? What's going on?
        if ((part_length & 0xc0) == 0xc0) {
            auto upper = part_length & 0x3f;
            if (position >  0) {
                if (name != nullptr) {
                    name[position++] = '.';
                }
                length++;
            }

            auto pointed = reader->beginning();
            auto offset = (upper << 8) | reader->read_u8();
            pointed.skip(offset);

            auto pointer_bytes = read_name(&pointed, name == nullptr ? nullptr : name + position);
            if (pointer_bytes.compressed < 0) {
                return { -1, -1 };
            }
            bytes++;
            position += pointer_bytes.name;
            length += pointer_bytes.name;
            break;
        }

        if (position > 0) {
            if (name != nullptr) {
                name[position++] = '.';
            }
            length++;
        }

        if (name != nullptr) {
            if (reader->read(name + position, part_length) != part_length) {
                return { -1, -1 };
            }
        }
        else {
            if (reader->skip(part_length) != part_length) {
                return { -1, -1 };
            }
        }

        // Move our pointer and the number of bytes.
        bytes += part_length;
        length += part_length;
        position += part_length;
    }
    return { (int16_t)bytes, (int16_t)length };
}

DNSReader::dns_name_t DNSReader::read_name(BufferedReader *reader) {
    auto searching = reader->remaining();
    // Read the name once to get the length and then we can allocate
    // memory for the name.
    auto lengths = read_name(&searching, nullptr);
    if (lengths.compressed < 0) {
        return { nullptr, -1 };
    }

    auto name = (uint8_t *)pool_->malloc(lengths.name + 1);
    read_name(reader, name);
    return { (char *)name, lengths.compressed };
}

int16_t DNSReader::parse() {
    loginfo("parsing %zu bytes q=%d a=%d au=%d ad=%d", size_, number_queries(), number_answers(),
            number_authorities(), number_additional());

    if (reader_.skip(sizeof(dns_header_t)) < 0) {
        return -1;
    }
    if (read_queries() < 0) {
        return -1;
    }
    if (read_answers() < 0) {
        return -1;
    }
    if (read_authorities() < 0) {
        return -1;
    }
    if (read_additionals() < 0) {
        return -1;
    }

    return 1;
}

uint16_t DNSReader::read_u16() {
    uint16_t value;
    if (reader_.read((uint8_t *)&value, sizeof(value)) != sizeof(value)) {
        error_ = true;
        return 0;
    }
    return ethutil_ntohs(value);
}

int16_t DNSReader::read_queries() {
    auto queries = ethutil_ntohs(header()->number_query);
    auto bytes = 0;
    while (queries > 0) {
        auto name = read_name(&reader_);

        auto record_type = read_u16();
        auto record_class = read_u16();

        loginfo("query: '%s' type=%d class=%d", name.name, record_type, record_class);

        queries--;
    }
    return bytes;
}

int16_t DNSReader::read_answers() {
    return read_records(number_answers());
}

int16_t DNSReader::read_authorities() {
    return read_records(number_authorities());
}

int16_t DNSReader::read_additionals() {
    return read_records(number_additional());
}

int16_t DNSReader::read_records(uint16_t number) {
    auto bytes = 0;
    while (number > 0) {
        auto name = read_name(&reader_);

        auto record_type = read_u16();
        auto record_class = read_u16();
        auto ttl1 = read_u16();
        auto ttl2 = read_u16();
        auto rdlength = read_u16();

        logdebug("answer: '%s' type=%d class=%d ttl=%d/%d", name.name, record_type, record_class, ttl1, ttl2);

        switch (record_type) {
        case (uint16_t)RecordTypes::A: {
            if (rdlength != 4) {
                error_ = true;
                return -1;
            }

            uint8_t ip[4];
            reader_.read((uint8_t *)&ip, sizeof(ip));

            logdebug("a record (%d) %d.%d.%d.%d", rdlength, ip[0], ip[1], ip[2], ip[3]);
            break;
        }
        case (uint16_t)RecordTypes::Aaaa: {
            logdebug("aaaa (ipv6) record (%d)", rdlength);
            reader_.skip(rdlength);
            break;
        }
        case (uint16_t)RecordTypes::Null: {
            logdebug("null record (%d)", rdlength);
            reader_.skip(rdlength);
            break;
        }
        case (uint16_t)RecordTypes::Srv: {
            auto priority = read_u16();
            auto weight = read_u16();
            auto port = read_u16();
            auto name = read_name(&reader_);
            if (name.name != nullptr) {
                logdebug("srv record (%d) pri=%d w=%d port=%d '%s'", rdlength, priority, weight, port, name.name);
            }
            else {
                logdebug("srv record (%d) pri=%d w=%d port=%d <noname>", rdlength, priority, weight, port);
            }
            break;
        }
        case (uint16_t)RecordTypes::Ptr: {
            auto name = read_name(&reader_);
            if (name.name != nullptr) {
                logdebug("ptr record (%d) '%s' (%d)", rdlength, name.name, name.length);
            }
            else {
                logdebug("ptr record (%d) <noname>", rdlength);
            }
            reader_.skip(rdlength - name.length);
            break;
        }
        default: {
            logdebug("unknown type (0x%x/%d) (rdlength=%d)", record_type, record_type, rdlength);
            reader_.skip(rdlength);
            break;
        }
        }

        number--;
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
