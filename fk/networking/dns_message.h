#pragma once

#include "pool.h"
#include "io.h"
#include "ethutil.h"
#include "collections.h"

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

class DNSReader {
private:
    Pool *pool_{ nullptr };
    uint8_t *buffer_{ nullptr };
    size_t size_{ 0 };
    bool error_{ true };
    BufferedReader reader_;
    collection<const char *> names_{ pool_ };

public:
    DNSReader(Pool *pool, uint8_t *ptr = nullptr, size_t size = 0);
    virtual ~DNSReader();

public:
    dns_header_t *header() const {
        return (dns_header_t *)buffer_;
    }

    dns_header_t *header() {
        return (dns_header_t *)buffer_;
    }

    collection<const char *> &names() {
        return names_;
    }

public:
    int16_t parse();

public:
    int16_t read_queries();
    int16_t read_answers();
    int16_t read_authorities();
    int16_t read_additionals();
    int16_t read_records(uint16_t number);

private:
    uint16_t number_queries() const {
        return ethutil_ntohs(header()->number_query);
    }

    uint16_t number_answers() const {
        return ethutil_ntohs(header()->number_answer);
    }

    uint16_t number_authorities() const {
        return ethutil_ntohs(header()->number_authority);
    }

    uint16_t number_additional() const {
        return ethutil_ntohs(header()->number_additional);
    }

public:
    struct dns_name_t {
        const char *name;
        int16_t length;
    };
    struct dns_name_length_t {
        int16_t compressed;
        int16_t name;
    };
    dns_name_length_t read_name(BufferedReader *reader, uint8_t *name);
    dns_name_t read_name(BufferedReader *reader);

    uint16_t read_u16();
};

class DNSWriter {
private:
    Pool *pool_{ nullptr };
    size_t size_{ 512 };
    uint8_t *buffer_{ nullptr };
    BufferedWriter writer_;

public:
    DNSWriter(Pool *pool);

public:
    void begin();
    EncodedMessage *finish();

public:
    dns_header_t *header() {
        return (dns_header_t *)buffer_;
    }

public:
    int32_t write_question(const char *name, uint16_t record_type, uint16_t record_class);
    int32_t write_answer_srv(const char *name, const char *server_name, uint16_t port);
    int32_t write_answer_ptr(const char *name, const char *ptr);

public:
    EncodedMessage *query_service_type(uint16_t xid, const char *service_type);
    EncodedMessage *answer_service_type(uint16_t xid, const char *service_type, const char *name);

};

} // namespace fk
