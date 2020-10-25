#pragma once

#include "pool.h"
#include "io.h"
#include "ethutil.h"

namespace fk {

typedef struct dns_header_t {
    uint16_t xid;
    uint8_t qr : 1;
    uint8_t opcode: 4;
    uint8_t authoritative : 1;
    uint8_t truncation : 1;
    uint8_t recursion_desired : 1;
    uint8_t recursion_available : 1;
    uint8_t reserved : 3;
    uint8_t response_code : 4;
    uint16_t number_query;
    uint16_t number_answer;
    uint16_t number_authority;
    uint16_t number_additional;
} __attribute__((__packed__)) dns_header_t;

class DNSMessage {
private:
    Pool *pool_{ nullptr };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    size_t position_{ 0 };
    dns_header_t *header_{ nullptr };
    bool error_{ true };

public:
    DNSMessage(Pool *pool, uint8_t *ptr = nullptr, size_t size = 0);
    virtual ~DNSMessage();

public:
    struct pointer_t {
        uint8_t *p;
        size_t moved;
    };
    struct dns_name_t {
        const char *name;
        int16_t length;
    };
    struct dns_name_length_t {
        int16_t compressed;
        int16_t name;
    };
    dns_name_length_t read_name(uint8_t *p, uint8_t *name, size_t size);
    dns_name_t read_name(uint8_t *p);

public:
    int16_t queries_size();
    int16_t answers_size();

public:
    uint16_t number_queries() const {
        return ethutil_ntohs(header_->number_query);
    }

    uint16_t number_answers() const {
        return ethutil_ntohs(header_->number_answer);
    }

    uint16_t number_authorities() const {
        return ethutil_ntohs(header_->number_authority);
    }

    uint16_t number_additional() const {
        return ethutil_ntohs(header_->number_additional);
    }

    uint16_t read_uint16(pointer_t &pos);

public:
    int16_t parse();

public:
    EncodedMessage *query_service_type(uint16_t xid, const char *service_type, Pool *pool);
    EncodedMessage *answer_service_type(uint16_t xid, const char *service_type, const char *name, Pool *pool);

private:
    uint8_t *end_of_packet() const {
        return buffer_ + size_;
    }

    uint8_t *after_header() const {
        return buffer_ + sizeof(dns_header_t);
    }
};

} // namespace fk
