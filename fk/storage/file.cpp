#include "storage/file.h"
#include "storage/storage.h"

#include <fk-data-protocol.h>

#include <phylum/crc.h>
#include <tiny_printf.h>

namespace fk {

FK_DECLARE_LOGGER("storage");

#if !defined(linux)
// #define FK_STORAGE_LOGGING_HASHING
#endif
#define FK_OP_STR_READ     "rd"
#define FK_OP_STR_WRITE    "wr"

static void log_hashed_data(const char *op, uint8_t file, uint32_t record, uint32_t address, void *data, size_t size) {
    #if defined(FK_STORAGE_LOGGING_HASHING)
    char prefix[32];
    tiny_snprintf(prefix, sizeof(prefix), "%s hash %5d 0x%06x ", op, record, address);
    fk_dump_memory(prefix, (const uint8_t *)data, size);
    #endif
}

File::File(Storage *storage, uint8_t file, FileHeader fh)
    : storage_(storage), file_(file), tail_(fh.tail), record_(fh.record), position_(0), size_(fh.size) {
    FK_ASSERT(file_ < NumberOfFiles);
}

File::~File() {
}

size_t File::write_record_header(size_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();

    auto left_in_block = g.remaining_in_block(tail_) - sizeof(BlockTail);
    auto total_required = sizeof(RecordHeader) + size + sizeof(RecordTail);

    if (!is_address_valid(tail_) || total_required > left_in_block) {
        if (is_address_valid(tail_)) {
            tail_ += left_in_block;
        }
        tail_ = storage_->allocate(file_, 0, tail_);
    }

    RecordHeader record_header;
    record_header.size = size;
    record_header.record = record_++;
    record_header.crc = record_header.sign();

    logverbose("[%d] 0x%06x write header (lib = %d) (%d bytes)", file_, tail_, left_in_block, size);

    if (memory.write(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
        return 0;
    }

    hash_.reset(Hash::Length);
    hash_.update(&record_header, sizeof(record_header));

    log_hashed_data(FK_OP_STR_WRITE, file_, record_ - 1, tail_, &record_header, sizeof(record_header));

    record_address_ = tail_;
    tail_ += sizeof(record_header);

    return sizeof(record_header);
}

size_t File::write_partial(uint8_t *record, size_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto left_in_block = (g.remaining_in_block(tail_) - sizeof(BlockTail));
    FK_ASSERT(left_in_block >= size);

    logverbose("[%d] 0x%06x write data (%d bytes) (%d lib)", file_, tail_, size, left_in_block);

    if (memory.write(tail_, (uint8_t *)record, size) != size) {
        return 0;
    }

    hash_.update(record, size);

    log_hashed_data(FK_OP_STR_WRITE, file_, record_ - 1, tail_, record, size);

    tail_ += size;

    return size;
}

size_t File::write_record_tail(size_t size) {
    SequentialMemory memory{ storage_->memory_ };

    RecordTail record_tail;
    record_tail.size = size;
    hash_.finalize(record_tail.hash.hash, sizeof(record_tail.hash.hash));
    if (memory.write(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return 0;
    }

    logverbose("[%d] 0x%06x write footer", file_, tail_);

    #if defined(FK_STORAGE_LOGGING_HASHING)
    char buffer[Hash::Length * 2];
    bytes_to_hex_string(buffer, sizeof(buffer), record_tail.hash.hash, Hash::Length);
    logdebug("[%d] 0x%06x hash(#%d) %s", file_, record_address_, record_ - 1, buffer);
    #endif

    tail_ += sizeof(record_tail);
    size_ += size;
    position_ += size;

    return sizeof(record_tail);
}

size_t File::write(uint8_t *record, size_t size) {
    SequentialMemory memory{ storage_->memory_ };

    logtrace("[%d] 0x%06x BEGIN write (%d bytes) #%d (%d w/ overhead)", file_, tail_, size, record_,
             sizeof(RecordHeader) + sizeof(RecordTail) + size);

    if (write_record_header(size) == 0) {
        return 0;
    }

    if (write_partial(record, size) != size) {
        return 0;
    }

    if (write_record_tail(size) == 0) {
        return 0;
    }

    update();

    return size;
}

size_t File::seek(uint32_t record) {
    auto sv = storage_->seek(SeekSettings{ file_, record });
    if (!sv.valid()) {
        return false;
    }

    tail_ = sv.address;
    record_ = sv.record;
    position_ = sv.position;
    record_remaining_ = 0;
    if (record == LastRecord) {
        size_ = position_;
    }

    return true;
}

size_t File::read_record_header() {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
    auto minimum_record_size = sizeof(RecordHeader) + sizeof(RecordTail);

    for (auto i = 0; i < 3; ++i) {
        if (left_in_block < minimum_record_size) {
            tail_ += left_in_block;

            BlockTail block_tail;
            if (memory.read(tail_, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
                return 0;
            }

            logverbose("[%d] 0x%06x btail (0x%06x)", file_, tail_, block_tail.linked);

            tail_ += sizeof(BlockTail);

            if (!is_address_valid(block_tail.linked)) {
                return 0;
            }

            tail_ = block_tail.linked;

            BlockHeader block_header;
            if (memory.read(tail_, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
                return 0;
            }

            FK_ASSERT(block_header.verify_hash());

            FK_ASSERT(block_header.file == file_);

            tail_ += sizeof(BlockHeader);
            left_in_block = g.remaining_in_block(tail_) - sizeof(BlockTail);
        }
        else {
            logverbose("[%d] 0x%06x trying header", file_, tail_);

            RecordHeader record_header;
            if (memory.read(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return 0;
            }

            if (!record_header.valid()) {
                loginfo("[%d] 0x%06x invalid header", file_, tail_);
                tail_ += left_in_block;
                left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
                continue;
            }

            record_ = record_header.record;
            record_remaining_ = record_header.size;
            record_address_ = tail_;

            hash_.reset(Hash::Length);
            hash_.update(&record_header, sizeof(RecordHeader));

            log_hashed_data(FK_OP_STR_READ, file_, record_, tail_, &record_header, sizeof(RecordHeader));

            logverbose("[%d] 0x%06x record header (%d bytes) #%d", file_, tail_, record_remaining_, record_header.record);

            tail_ += sizeof(record_header);

            return record_remaining_;
        }
    }

    return 0;
}

size_t File::read(uint8_t *record, size_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
    auto bytes_read = (size_t)0;

    logtrace("[%d] 0x%06x BEGIN read (%d bytes) (rr = %d) (lib = %d)", file_, tail_, size, record_remaining_, left_in_block);

    while (bytes_read < size) {
        if (record_remaining_ == 0) {
            if (read_record_header() == 0) {
                return bytes_read;
            }

            left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
        }
        else if (bytes_read < size) {
            auto buffer_remaining = size - bytes_read;
            auto reading = std::min<size_t>(left_in_block, std::min<size_t>(buffer_remaining, record_remaining_));
            FK_ASSERT(reading > 0);
            if (memory.read(tail_, (uint8_t *)record + bytes_read, reading) != reading) {
                return bytes_read;
            }

            hash_.update(record + bytes_read, reading);

            log_hashed_data(FK_OP_STR_READ, file_, record_, tail_, record + bytes_read, reading);

            logverbose("[%d] 0x%06x data (%d bytes)", file_, tail_, reading);

            tail_ += reading;
            bytes_read += reading;
            left_in_block -= reading;
            record_remaining_ -= reading;
            position_ += reading;

            if (record_remaining_ == 0) {
                if (read_record_tail() == 0) {
                    return bytes_read;
                }

                left_in_block -= sizeof(RecordTail);
            }
        }
        else {
            break;
        }
    }

    return bytes_read;
}

size_t File::read_record_tail() {
    SequentialMemory memory{ storage_->memory_ };

    logverbose("[%d] 0x%06x end of record", file_, tail_);

    RecordTail record_tail;
    if (memory.read(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return 0;
    }

    // TODO: We can recover from this better.
    Hash hash;
    hash_.finalize(&hash.hash, Hash::Length);
    if (memcmp(hash.hash, record_tail.hash.hash, Hash::Length) != 0) {
        logerror("[%d] 0x%06x hash mismatch: (#%d) (record address = 0x%06x)", file_, tail_, record_, record_address_);
        if (false) {
            fk_dump_memory("GOOD ", record_tail.hash.hash, Hash::Length);
            fk_dump_memory("BAD  ", hash.hash, Hash::Length);
        }
        number_hash_errors_++;
    }

    tail_ += sizeof(RecordTail);

    return sizeof(RecordTail);
}

void File::update() {
    auto &fh = storage_->files_[file_];
    fh.tail = tail_;
    fh.size = size_;
    fh.record = record_;
}

typedef struct pb_file_t {
    // TODO: MALLOC
    uint8_t buffer[1024];
    size_t position;
    size_t buffer_size;
    size_t record_size;
    size_t bytes_read;
    File *file;

    bool flush() {
        if (position == 0) {
            return true;
        }

        if (file->write_partial(buffer, position) != position) {
            return false;
        }

        position = 0;

        return true;
    }
} pb_file_t;

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto pbf = reinterpret_cast<pb_file_t*>(stream->state);
    auto available = pbf->buffer_size - pbf->position;
    if (c > available) {
        if (!pbf->flush()) {
            return false;
        }
    }

    memcpy(pbf->buffer + pbf->position, buf, c);
    pbf->position += c;

    return true;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto pbf = reinterpret_cast<pb_file_t*>(stream->state);
    if (pbf->bytes_read == 0) {
        auto reading = std::min<size_t>(pbf->buffer_size, pbf->record_size);
        if (pbf->file->read(pbf->buffer, reading) != reading) {
            return false;
        }

        pbf->position = 0;
        pbf->bytes_read = reading;
    }

    FK_ASSERT(pbf->position + c <= pbf->bytes_read);

    memcpy(buf, pbf->buffer + pbf->position, c);
    pbf->position += c;

    return true;
}

pb_ostream_t pb_ostream_from_file(pb_file_t *pbf) {
    return { &write_callback, (void *)pbf, SIZE_MAX, 0 };
}

pb_istream_t pb_istream_from_file(pb_file_t *pbf, size_t size) {
    return { &read_callback, (void *)pbf, size };
}

size_t File::write(void *record, const pb_msgdesc_t *fields) {
    pb_file_t pbf;
    pbf.buffer_size = sizeof(pbf.buffer);
    pbf.record_size = 0;
    pbf.position = 0;
    pbf.bytes_read = 0;
    pbf.file = this;

    if (!pb_get_encoded_size(&pbf.record_size, fields, record)) {
        return 0;
    }

    if (write_record_header(pbf.record_size) == 0) {
        return 0;
    }

    pb_ostream_t ostream = pb_ostream_from_file(&pbf);
    if (!pb_encode(&ostream, fields, record)) {
        return 0;
    }

    if (!pbf.flush()) {
        return 0;
    }

    if (write_record_tail(pbf.record_size) == 0) {
        return 0;
    }

    update();

    return pbf.record_size;
}

size_t File::read(void *record, const pb_msgdesc_t *fields) {
    pb_file_t pbf;
    pbf.buffer_size = sizeof(pbf.buffer);
    pbf.record_size = 0;
    pbf.position = 0;
    pbf.bytes_read = 0;
    pbf.file = this;

    pbf.record_size = read_record_header();
    if (pbf.record_size == 0) {
        return 0;
    }

    pb_istream_t istream = pb_istream_from_file(&pbf, pbf.record_size);
    if (!pb_decode(&istream, fields, record)) {
        return 0;
    }

    return pbf.record_size;
}

}
