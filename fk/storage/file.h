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
    StorageAddress address{ InvalidAddress };
    uint32_t position{ 0 };
    RecordNumber record{ InvalidRecord };
    uint32_t record_size{ 0 };

    RecordReference() {
    }

    RecordReference(StorageAddress address, uint32_t position, RecordNumber record, uint32_t record_size) : address(address), position(position), record(record), record_size(record_size) {
    }

    bool valid() const {
        return address != InvalidAddress;
    }
};

class File : public Writer, public Reader {
private:
    Storage *storage_;
    SequentialWrapper<BufferedPageMemory> memory_;
    FileNumber file_;
    StorageAddress record_address_{ InvalidAddress };
    StorageAddress tail_{ InvalidAddress };
    RecordNumber record_{ InvalidRecord };
    StorageVersion version_{ InvalidVersion };
    RecordSize record_remaining_{ 0 };
    RecordSize record_size_{ 0 };
    StorageSize position_{ 0 };
    StorageSize size_{ 0 };
    uint32_t number_hash_errors_{ 0 };
    StorageSize bytes_in_block_{ 0 };
    RecordNumber records_in_block_{ 0 };
    StorageSize wasted_{ 0 };
    bool written_{ false };
    bool partial_allowed_{ false };
    bool unread_header_{ false };
    BLAKE2b hash_;

public:
    File(Storage *storage, FileNumber file, Pool &pool);
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
    bool seek(RecordNumber record);
    bool seek(RecordReference reference);
    bool beginning_of_record();
    int32_t skip(bool new_block = false);
    int32_t rewind();
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

    BlockNumber tail_block() const {
        if (tail_ == InvalidAddress) {
            return 0;
        }
        return tail_ / geometry().block_size;
    }

    uint32_t record_address() const {
        return record_address_;
    }

    uint32_t record_size() const {
        return record_size_;
    }

    uint32_t record() const {
        return record_;
    }

    uint32_t end_record() const {
        if (written_) {
            return record_ - 1;
        }
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

    bool at_start_of_file() const;

private:
    int32_t find_following_block();
    uint32_t find_previous_sector_aligned_record(StorageAddress address);
    int32_t write_record_header(RecordSize size);
    int32_t write_record_tail(RecordSize size);
    int32_t read_record_header();
    int32_t read_record_tail();
    int32_t search_for_following_block();
    void update();
    bool partial_write_align_necessary() const;

public:
    int32_t try_write(uint8_t const *record, size_t size);
    int32_t try_write(void const *record, pb_msgdesc_t const *fields);

    struct SizeInfo {
        StorageSize size;
        BlockNumber last_block;
    };

    SizeInfo get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool);
    void walk(BlockNumber first_block, BlockNumber last_block, Pool &pool);

private:
    int32_t try_read_record_header(StorageAddress tail, RecordHeader &record_header);
};

} // namespace fk
