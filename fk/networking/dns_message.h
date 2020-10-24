#pragma once

#include "pool.h"
#include "ethutil.h"

namespace fk {

typedef struct dns_header_t {
    uint16_t xid;
    uint8_t recursion_desired : 1;
    uint8_t truncated : 1;
    uint8_t authoritive_answer : 1;
    uint8_t op_code : 4;
    uint8_t query_response : 1;
    uint8_t response_code : 4;
    uint8_t checking_disabled : 1;
    uint8_t authenticated_data : 1;
    uint8_t reserved : 1;
    uint8_t recursion_available : 1;
    uint16_t number_query;
    uint16_t number_answer;
    uint16_t number_authority;
    uint16_t number_additional;
} __attribute__((__packed__)) dns_header_t;

class DnsMessage {
private:
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    dns_header_t *header_{ nullptr };
    bool error_{ true };

public:
    DnsMessage(uint8_t *ptr, size_t size);
    virtual ~DnsMessage();

public:
    int16_t queries_size();
    int16_t answers_size();
    int16_t read_name(uint8_t *p);
    int16_t read_names(uint8_t *p);

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


public:
    bool parse(Pool *pool);

private:
    uint8_t *end_of_packet() const {
        return buffer_ + size_;
    }

    uint8_t *after_header() const {
        return buffer_ + sizeof(dns_header_t);
    }
};

} // namespace fk
