#include "platform.h"
#include "common.h"
#include "networking/dns_message.h"
#include "ethutil.h"

namespace fk {

int16_t DnsMessage::read_name(uint8_t *p) {
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

        auto name = strndup((char *)p + 1, part_length + 1); // MEM
        name[part_length] = 0;
        printf("plength: '%s' (%d)\n", name, part_length);
        p += part_length + 1;
        bytes += part_length + 1;
    }
    return bytes;
}

DnsMessage::DnsMessage(uint8_t *buffer, size_t size) : buffer_(buffer), size_(size), header_((dns_header_t *)buffer_) {
}

DnsMessage::~DnsMessage() {
}

bool DnsMessage::parse(Pool *pool) {
    /*auto queries_bytes =*/ queries_size();
    /*auto answers_bytes =*/ answers_size();

    return true;
}

int16_t DnsMessage::queries_size() {
    auto p = after_header();
    auto queries = ethutil_ntohs(header_->number_query);
    auto bytes = 0;
    while (queries > 0) {
        auto record_length = read_name(p);
        p += record_length;
        // auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        // auto record_class = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;

        record_length += 2 * 2;
        bytes += record_length;
        queries--;
    }
    return bytes;
}

int16_t DnsMessage::answers_size() {
    auto p = after_header() + queries_size();
    auto answers = ethutil_ntohs(header_->number_answer);
    auto bytes = 0;
    while (answers > 0) {
        auto record_length = read_name(p);
        p += record_length;

        auto record_type = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        // auto record_class = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        // auto ttl1 = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        // auto ttl2 = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;
        auto rdlength = ethutil_ntohs(*(uint16_t *)&p[0]);
        p += 2;

        switch (record_type) {
        case 0x01: {
            if (rdlength != 4) {
                error_ = true;
                return -1;
            }

            uint8_t ip[4];
            memcpy(ip, p, 4);

            printf("a record (%d) %d.%d.%d.%d\n", rdlength, ip[0], ip[1], ip[2], ip[3]);
            p += rdlength;
            break;
        }
        case 0x0c: {
            printf("ptr record (%d)\n", rdlength);

            /*auto name_length = */read_name(p);

            p += rdlength;
            break;
        }
        default: {
            printf("unknown type (0x%x) (rdlength=%d)\n", record_type, rdlength);
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

} // namespace fk
