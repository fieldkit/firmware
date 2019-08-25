#pragma once

#include "common.h"
#include "storage/types.h"

#include <phylum/blake2b.h>
#include <pb_encode.h>
#include <pb_decode.h>

namespace fk {

class Storage;

struct RecordReference {
    uint32_t address{ InvalidAddress };
    uint32_t position{ 0 };
    uint32_t record{ InvalidRecord };

    RecordReference() {
    }

    RecordReference(uint32_t address, uint32_t position, uint32_t record) : address(address), position(position), record(record) {
    }

    bool valid() const {
        return address != InvalidAddress;
    }
};

class File {
private:
    Storage *storage_;
    uint8_t file_;
    uint32_t record_address_{ InvalidAddress };
    uint32_t tail_{ InvalidAddress };
    uint32_t record_{ InvalidRecord };
    uint32_t version_{ InvalidVersion };
    uint32_t record_remaining_{ 0 };
    uint32_t record_size_{ 0 };
    uint32_t position_{ 0 };
    uint32_t size_{ 0 };
    uint32_t number_hash_errors_{ 0 };
    uint32_t bytes_in_block_{ 0 };
    uint32_t records_in_block_{ 0 };
    BLAKE2b hash_;

public:
    File(Storage *storage, uint8_t file);
    virtual ~File();

public:
    bool create();
    bool seek_end();
    bool seek(uint32_t record);
    bool seek(RecordReference reference);
    size_t write(uint8_t *record, size_t size);
    size_t read(uint8_t *record, size_t size);
    size_t write(void *record, const pb_msgdesc_t *fields);
    size_t read(void *record, const pb_msgdesc_t *fields);
    size_t write_partial(uint8_t *record, size_t size);
    bool beginning_of_record();

public:
    RecordReference reference() const {
        return {
            .address = record_address_,
            .position = position_ - record_size_,
            .record = record_,
        };
    }

    uint8_t file_number() const {
        return file_;
    }

    uint32_t tail() const {
        return tail_;
    }

    uint32_t record_address() const {
        return record_address_;
    }

    uint32_t record() const {
        return record_;
    }

    uint32_t previous_record() const {
        return record_ - 1;
    }

    uint32_t position() const {
        return position_;
    }

    uint32_t size() const {
        return size_;
    }

    bool valid() const {
        return is_address_valid(tail_);
    }

    uint32_t number_of_hash_errors() const {
        return number_hash_errors_;
    }

private:
    size_t write_record_header(size_t size);
    size_t write_record_tail(size_t size);
    size_t read_record_header();
    size_t read_record_tail();
    void update();

};

}
