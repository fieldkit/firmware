#pragma once

#include "common.h"
#include "storage/types.h"
#include "storage/sequential_memory.h"

#include <phylum/blake2b.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "io.h"

namespace fk {

class Storage;

struct RecordReference {
    uint32_t address{ InvalidAddress };
    uint32_t position{ 0 };
    uint32_t record{ InvalidRecord };
    uint32_t record_size{ 0 };

    RecordReference() {
    }

    RecordReference(uint32_t address, uint32_t position, uint32_t record, uint32_t record_size) : address(address), position(position), record(record), record_size(record_size) {
    }

    bool valid() const {
        return address != InvalidAddress;
    }
};

class File : public Writer, public Reader {
private:
    Storage *storage_;
    SequentialWrapper<CacheSinglePageMemory> memory_;
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
    uint32_t wasted_{ 0 };
    bool partial_allowed_{ false };
    BLAKE2b hash_;

public:
    File(Storage *storage, uint8_t file);
    File(File &&o);
    File(File const &o) = delete;
    virtual ~File();

public:
    File &operator=(File const &o) = delete;
    File &operator=(File &&o);

public:
    bool create();
    bool seek_end();
    bool seek_beginning();
    bool seek(uint32_t record);
    bool seek(RecordReference reference);
    bool skip(bool new_block = false);
    bool rewind();
    bool beginning_of_record();
    int32_t write(uint8_t const *record, size_t size) override;
    int32_t read(uint8_t *record, size_t size) override;
    int32_t write(void const *record, pb_msgdesc_t const *fields);
    int32_t read(void *record, pb_msgdesc_t const *fields);

public:
    int32_t write_partial(uint8_t const *record, size_t size);

public:
    RecordReference reference() const;

    FlashGeometry geometry() const;

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

    uint32_t wasted() const {
        return wasted_;
    }

    bool valid() const {
        return is_address_valid(tail_);
    }

    uint32_t number_of_hash_errors() const {
        return number_hash_errors_;
    }

private:
    int32_t find_following_block();
    int32_t write_record_header(size_t size);
    int32_t write_record_tail(size_t size);
    int32_t read_record_header();
    int32_t read_record_tail();
    int32_t search_for_following_block();
    void update();
    bool partial_write_align_necessary() const;

private:
    int32_t try_read_record_header(uint32_t tail, RecordHeader &record_header);

};

}
