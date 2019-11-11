#include <algorithm>
#include <tiny_printf.h>
#include <phylum/crc.h>
#include <fk-data-protocol.h>

#include "platform.h"
#include "hal/random.h"
#include "storage/file.h"
#include "storage/storage.h"
#include "utilities.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("storage");

#if !defined(linux)
// #define FK_STORAGE_LOGGING_HASHING
#endif
#define FK_OP_STR_READ     "rd"
#define FK_OP_STR_WRITE    "wr"

static void log_hashed_data(const char *op, uint8_t file, uint32_t record, uint32_t address, void const *data, size_t size) {
    #if defined(FK_STORAGE_LOGGING_HASHING)
    char prefix[32];
    tiny_snprintf(prefix, sizeof(prefix), "%s hash %5d " PRADDRESS " ", op, record, address);
    fk_dump_memory(prefix, (const uint8_t *)data, size);
    #endif
}

File::File(Storage *storage, uint8_t file)
    : storage_(storage), memory_(storage->data_memory_), file_(file), version_{ storage->version_ }, position_(0) {
    FK_ASSERT(file_ < NumberOfFiles);
}

File::File(File &&o)
    : storage_{ o.storage_ }, memory_{ std::move(o.memory_) }, file_{ o.file_ },
      record_address_{ o.record_address_ }, tail_{ o.tail_ }, record_{ o.record_ }, version_{ o.version_ },
      record_remaining_{ o.record_remaining_ }, record_size_{ o.record_size_ }, position_{ o.position_ },
      size_{ o.size_ }, number_hash_errors_{ o.number_hash_errors_ }, bytes_in_block_{ o.bytes_in_block_ },
      records_in_block_{ o.records_in_block_ }, wasted_{ o.wasted_ }, partial_allowed_{ o.partial_allowed_ },
      hash_{ o.hash_ } {
} // namespace fk

File::~File() {
}

File &File::operator=(File &&o) {
    storage_ = o.storage_;
    memory_ = std::move(o.memory_);
    file_ = o.file_;
    record_address_ = o.record_address_;
    tail_ = o.tail_;
    record_ = o.record_;
    version_ = o.version_;
    record_remaining_ = o.record_remaining_;
    record_size_ = o.record_size_;
    position_ = o.position_;
    size_ = o.size_;
    number_hash_errors_ = o.number_hash_errors_;
    bytes_in_block_ = o.bytes_in_block_;
    records_in_block_ = o.records_in_block_;
    wasted_ = o.wasted_;
    partial_allowed_ = o.partial_allowed_;
    hash_ = o.hash_;
    return *this;
}

FlashGeometry File::geometry() const {
    return storage_->memory_.geometry();
}

bool File::beginning_of_record() {
    FK_ASSERT(record_address_ != InvalidAddress);

    tail_ = record_address_;

    return true;
}

bool File::partial_write_align_necessary() const {
    return true;
}

int32_t File::write_record_header(size_t size) {
    auto g = geometry();

    auto total_required = sizeof(RecordHeader) + size + sizeof(RecordTail);
    auto left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;

    // Can we write to the end of the previous header or do we need to partial
    // write align this record?
    if (is_address_valid(tail_)) {
        if (partial_write_align_necessary()) {
            auto previous_tail = tail_;
            tail_ = g.partial_write_boundary_after(tail_);

            if (g.is_start_of_block(tail_))  {
                tail_ -= SizeofBlockTail;
                left_in_block = 0;
            }
            else {
                left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
                wasted_ += tail_ - previous_tail;
            }
        }
    }

    if (!is_address_valid(tail_) || total_required > left_in_block) {
        auto block_tail_address = tail_;

        if (is_address_valid(tail_)) {
            tail_ += left_in_block;
            // FK_ASSERT(bytes_in_block_ > 0);
            // FK_ASSERT(records_in_block_ > 0);
        }

        BlockTail block_tail;
        block_tail.bytes_in_block = bytes_in_block_;
        block_tail.records_in_block = records_in_block_;
        block_tail.block_tail = block_tail_address;
        tail_ = storage_->allocate(file_, tail_, block_tail);
        FK_ASSERT(tail_ != InvalidAddress);
        if (record_ == InvalidRecord) {
            record_ = 1;
        }
        left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
        bytes_in_block_ = 0;
        records_in_block_ = 0;
    }

    RecordHeader record_header;
    record_header.size = size;
    record_header.record = record_++;
    record_header.previous = record_address_;
    record_header.crc = record_header.sign();

    logdebug("[%d] " PRADDRESS " write header #%" PRIu32 " (lib=%" PRId32 ") (%zd bytes) position=%" PRIu32 " sopr=" PRADDRESS,
             file_, tail_, record_header.record, (int32_t)left_in_block, size, position_, record_address_);

    if (memory_.write(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
        return 0;
    }

    hash_.reset(Hash::Length);
    hash_.update(&record_header, sizeof(record_header));

    log_hashed_data(FK_OP_STR_WRITE, file_, record_ - 1, tail_, &record_header, sizeof(record_header));

    record_address_ = tail_;
    record_remaining_ = 0;
    record_size_ = 0;
    tail_ += sizeof(record_header);

    return sizeof(record_header);
}

int32_t File::write_partial(uint8_t const *record, size_t size) {
    storage_->verify_mutable();

    auto g = geometry();
    auto left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
    FK_ASSERT(left_in_block >= size);

    logverbose("[%d] " PRADDRESS " write data (%zd bytes) (%" PRIu32 " lib)", file_, tail_, size, (int32_t)left_in_block);

    if (memory_.write(tail_, (uint8_t *)record, size) <= 0) {
        return 0;
    }

    hash_.update(record, size);

    log_hashed_data(FK_OP_STR_WRITE, file_, record_ - 1, tail_, record, size);

    tail_ += size;
    record_size_ += size;
    size_ += size;
    position_ += size;

    return size;
}

int32_t File::write_record_tail(size_t size) {
    RecordTail record_tail;
    record_tail.size = size;
    hash_.finalize(record_tail.hash.hash, sizeof(record_tail.hash.hash));
    if (memory_.write(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return 0;
    }

    logverbose("[%d] " PRADDRESS " write footer 0x%06" PRIx32, file_, tail_, (uint32_t)(tail_ + sizeof(record_tail)));

    #if defined(FK_STORAGE_LOGGING_HASHING)
    char buffer[Hash::Length * 2];
    bytes_to_hex_string(buffer, sizeof(buffer), record_tail.hash.hash, Hash::Length);
    logdebug("[%d] 0x%06x hash(#%" PRIu32 ") %s", file_, record_address_, record_ - 1, buffer);
    #endif

    tail_ += sizeof(record_tail);
    bytes_in_block_ += size;
    records_in_block_++;

    return sizeof(record_tail);
}

int32_t File::try_write(uint8_t const *record, size_t size) {
    storage_->verify_mutable();

    logtrace("[%d] " PRADDRESS " BEGIN write (%zd bytes) #%" PRIu32 " (%" PRIu32 " w/ overhead)", file_, tail_, size, record_,
             (uint32_t)(sizeof(RecordHeader) + sizeof(RecordTail) + size));

    if (write_record_header(size) == 0) {
        return 0;
    }

    auto wrote = write_partial(record, size);
    if (wrote != (int32_t)size) {
        return 0;
    }

    if (write_record_tail(size) == 0) {
        return 0;
    }

    if (memory_.flush() <= 0) {
        // NOTE: This works because we only ever write a record to a
        // single block. So where the file pointer is, that's the bad one.
        storage_->bad_blocks_.mark_address_as_bad(tail_);
        return 0;
    }

    update();

    return size;
}

bool File::seek_end() {
    return seek(LastRecord);
}

RecordReference File::reference() const {
    return {
        .address = record_address_,
        .position = position_ - record_size_,
        .record = record_,
        .record_size = record_size_,
    };
}

bool File::seek_beginning() {
    return seek(0);
}

bool File::seek(RecordReference reference) {
    wasted_ = 0;
    tail_ = reference.address;
    position_ = reference.position;
    record_ = reference.record;
    record_size_ = reference.record_size;

    // This forces us to reread the header of the record, which clears the hash state.
    record_remaining_ = 0;

    // NOTE We're usually reading when doing this.
    bytes_in_block_ = 0;
    records_in_block_ = 0;

    logtrace("[" PRADDRESS "] seek reference position = %" PRIu32 " record = #%" PRIu32 " size=%" PRIu32,
             tail_, position_, record_, record_size_);

    return true;
}

bool File::create() {
    record_ = 1;
    position_ = 0;
    size_ = 0;
    record_remaining_ = 0;
    record_size_ = 0;
    tail_ = InvalidAddress;
    return true;
}

bool File::seek(uint32_t record) {
    auto sv = storage_->seek(SeekSettings{ file_, record });
    if (!sv.valid()) {
        return false;
    }

    wasted_ = 0;
    tail_ = sv.address;
    record_ = sv.record;
    position_ = sv.position;
    record_remaining_ = 0;
    record_size_ = 0;
    record_address_ = sv.record_address;
    bytes_in_block_ = sv.bytes_in_block;
    records_in_block_ = sv.records_in_block;
    if (record == LastRecord) {
        size_ = position_;
        update();
    }

    return true;
}

bool File::skip(bool new_block) {
    auto g = storage_->geometry();

    logtrace("[" PRADDRESS "] skip", tail_);

    RecordHeader record_header;
    if (memory_.read(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
        return false;
    }

    if (!record_header.valid()) {
        if (new_block) {
            return false;
        }

        auto partial_aligned = g.partial_write_boundary_after(tail_);
        if (memory_.read(partial_aligned, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
            return false;
        }

        if (!record_header.valid()) {
            loginfo("[" PRADDRESS "] skip invalid record header", tail_);

            auto left_in_block = (uint32_t)(g.remaining_in_block(tail_));
            auto head_address = tail_ + left_in_block;

            BlockHeader block_header;
            if (memory_.read(head_address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
                return 0;
            }

            tail_ = head_address + SizeofBlockHeader;

            return skip(true);
        }

        wasted_ += partial_aligned - tail_;
        tail_ = partial_aligned;
    }

    tail_ += sizeof(RecordHeader);

    tail_ += record_header.size;

    RecordTail record_tail;
    if (memory_.read(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return false;
    }

    tail_ += sizeof(RecordTail);

    record_ = record_header.record;
    record_size_ = record_header.size;
    record_remaining_ = record_header.size;
    position_ -= record_tail.size;

    return true;
}

uint32_t File::find_previous_sector_aligned_record(uint32_t address) {
    auto g = storage_->geometry();

    while (!g.is_start_of_block_or_header(address, SizeofBlockHeader)) {
        address = g.partial_write_boundary_before(address);

        RecordHeader record_header;
        if (memory_.read(address, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
            logerror("[" PRADDRESS "] error reading", tail_);
            return InvalidAddress;
        }

        if (record_header.valid()) {
            return address;
        }

        address -= g.sector_size();
    }

    return InvalidAddress;
}

bool File::rewind() {
    auto g = storage_->geometry();
    auto start_of_record = tail_;

    logtrace("[" PRADDRESS "] rewind", tail_);

    if (g.is_start_of_block_or_header(tail_, SizeofBlockHeader)) {
        auto start = g.start_of_block(tail_);
        if (start == 0) {
            tail_ = 0;
            logtrace("[" PRADDRESS "] rewind start of block", tail_);
            return false;
        }

        tail_ = g.start_of_block(start_of_record) - SizeofBlockTail;

        logdebug("[" PRADDRESS "] rewind spans block, reading tail", tail_);

        BlockTail block_tail;
        if (memory_.read(tail_, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
            return false;
        }

        if (!block_tail.verify_hash()) {
            logdebug("[" PRADDRESS "] invalid block tail", tail_);
            return false;
        }

        tail_ = block_tail.block_tail;

        FK_ASSERT(g.is_address_valid(tail_));
    }

    tail_ -= sizeof(RecordTail);

    RecordTail record_tail;
    if (memory_.read(tail_, (uint8_t *)&record_tail, sizeof(record_tail)) != sizeof(record_tail)) {
        logerror("[" PRADDRESS "] error reading", tail_);
        return false;
    }

    if (record_tail.valid()) {
        tail_ -= record_tail.size;

        tail_ -= sizeof(RecordHeader);
    }
    else {
        uint32_t previous_record = find_previous_sector_aligned_record(tail_);
        if (previous_record == InvalidAddress) {
            logerror("[" PRADDRESS "] rewind invalid record tail, unable to find previous record in block", tail_);
            return false;
        }

        tail_ = previous_record;
    }

    RecordHeader record_header;
    if (memory_.read(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
        logerror("[" PRADDRESS "] error reading", tail_);
        return false;
    }

    if (!record_header.valid()) {
        logerror("[" PRADDRESS "] rewind invalid record header", tail_);
        return false;
    }

    record_ = record_header.record;
    record_size_ = record_header.size;
    record_remaining_ = record_header.size;
    position_ -= record_tail.size;

    logtrace("[" PRADDRESS "] rewind DONE position=%" PRIu32 " record-size=%" PRIu32, tail_, position_,
             record_tail.size);

    return true;
}

int32_t File::search_for_following_block() {
    auto g = geometry();

    auto iter = g.truncate_to_block(tail_) + g.block_size;

    loginfo("[%d]" PRADDRESS " searching for following block", file_, iter);

    while (g.is_address_valid(iter)) {
        BlockHeader block_header;
        if (memory_.read(iter, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            return 0;
        }

        if (!block_header.verify_hash() || block_header.version != version_) {
            loginfo("[%d]" PRADDRESS " invalid block header", file_, iter);
            return 0;
        }
        if (block_header.file == file_) {
            loginfo("[%d]" PRADDRESS " found", file_, iter);
            tail_ = iter;
            return tail_;
        }

        iter += g.block_size;
    }

    return 0;
}

int32_t File::find_following_block() {
    auto g = geometry();
    auto left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;

    auto started = tail_;

    tail_ += left_in_block;

    BlockTail block_tail;
    if (memory_.read(tail_, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
        return 0;
    }

    logdebug("[%d] " PRADDRESS " btail (" PRADDRESS ")", file_, tail_, block_tail.linked);

    if (!is_address_valid(block_tail.linked)) {
        if (search_for_following_block() == 0) {
            logerror("[%d] " PRADDRESS " unable to resume", file_, tail_);
            return 0;
        }
    }

    if (!block_tail.verify_hash()) {
        logerror("[%d] " PRADDRESS " btail failed hash (" PRADDRESS ")", file_, tail_, block_tail.linked);
        fk_dump_memory("ACT ", (uint8_t *)&block_tail.hash, sizeof(block_tail.hash));
        block_tail.fill_hash();
        fk_dump_memory("EXP ", (uint8_t *)&block_tail.hash, sizeof(block_tail.hash));

        if (search_for_following_block() == 0) {
            logerror("[%d] " PRADDRESS " unable to resume", file_, tail_);
            return 0;
        }
    }
    else {
        tail_ = block_tail.linked;
    }

    BlockHeader block_header;
    if (memory_.read(tail_, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
        return 0;
    }

    FK_ASSERT(block_header.version == version_);
    FK_ASSERT(block_header.verify_hash());
    FK_ASSERT(block_header.file == file_);

    tail_ += SizeofBlockHeader;

    return tail_ - started;
}

int32_t File::try_read_record_header(uint32_t tail, RecordHeader &record_header) {
    logverbose("[%d] " PRADDRESS " trying header", file_, tail);

    if (memory_.read(tail, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
        return 0;
    }

    if (!record_header.valid()) {
        logtrace("[%d] " PRADDRESS " invalid header", file_, tail);
        return 0;
    }

    logverbose("[%d] " PRADDRESS " record header (%" PRIu32 " bytes) #%" PRIu32, file_, tail, record_header.size, record_header.record);

    return record_header.size;
}

int32_t File::read_record_header() {
    auto g = geometry();
    auto left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
    auto minimum_record_size = sizeof(RecordHeader) + sizeof(RecordTail);

    /**
     * Where record headers can be:
     * 1) At tail_
     * 2) First partial page boundary after tail_
     * 3) At beginning of next valid block
     */

    for (auto i = 0; i < 3; ++i) {
        if (left_in_block < minimum_record_size) {
            if (!find_following_block()) {
                return 0;
            }

            left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
        }
        else {
            RecordHeader record_header;
            if (!try_read_record_header(tail_, record_header)) {
                auto partial_aligned = g.partial_write_boundary_after(tail_);
                if (!try_read_record_header(partial_aligned, record_header)) {
                    tail_ += left_in_block;
                    left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
                    continue;
                }

                wasted_ += partial_aligned - tail_;
                tail_ = partial_aligned;
            }

            record_ = record_header.record;
            record_remaining_ = record_header.size;
            record_address_ = tail_;

            hash_.reset(Hash::Length);
            hash_.update(&record_header, sizeof(RecordHeader));

            log_hashed_data(FK_OP_STR_READ, file_, record_, tail_, &record_header, sizeof(RecordHeader));

            tail_ += sizeof(RecordHeader);

            return record_remaining_;
        }
    }

    return 0;
}

int32_t File::read(uint8_t *record, size_t size) {
    auto g = geometry();
    auto left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
    auto bytes_read = (size_t)0;

    logtrace("[%d] " PRADDRESS " BEGIN read (%zd bytes) (rr = %" PRIu32 ") (lib = %" PRIu32 ")", file_, tail_, size, record_remaining_, left_in_block);

    while (bytes_read < size) {
        if (record_remaining_ == 0) {
            if (read_record_header() == 0) {
                return bytes_read;
            }

            left_in_block = g.remaining_in_block(tail_) - SizeofBlockTail;
        }
        else if (bytes_read < size) {
            auto buffer_remaining = size - bytes_read;
            auto reading = std::min<size_t>(left_in_block, std::min<size_t>(buffer_remaining, record_remaining_));
            FK_ASSERT(reading > 0);
            if (memory_.read(tail_, (uint8_t *)record + bytes_read, reading) <= 0) {
                return bytes_read;
            }

            hash_.update(record + bytes_read, reading);

            log_hashed_data(FK_OP_STR_READ, file_, record_, tail_, record + bytes_read, reading);

            logverbose("[%d] " PRADDRESS " data (%zd bytes)", file_, tail_, reading);

            tail_ += reading;
            bytes_read += reading;
            left_in_block -= reading;
            record_remaining_ -= reading;
            position_ += reading;

            if (record_remaining_ == 0) {
                if (read_record_tail() == 0) {
                    logwarn("[%d] " PRADDRESS " error reading tail", file_, tail_);
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

int32_t File::read_record_tail() {
    logverbose("[%d] " PRADDRESS " record tail (eor = " PRADDRESS ")", file_, tail_, (uint32_t)(tail_ + sizeof(RecordTail)));

    RecordTail record_tail;
    if (memory_.read(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return 0;
    }

    // TODO: We can recover from this better.
    Hash hash;
    hash_.finalize(&hash.hash, Hash::Length);
    if (memcmp(hash.hash, record_tail.hash.hash, Hash::Length) != 0) {
        logerror("[%d] " PRADDRESS " hash mismatch: (#%" PRIu32 ") (record address = " PRADDRESS ") (record_size = %" PRIu32 ")",
                 file_, tail_, record_, record_address_, record_tail.size);
        fk_dump_memory("ACT ", record_tail.hash.hash, Hash::Length);
        fk_dump_memory("EXP ", hash.hash, Hash::Length);
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

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto file = reinterpret_cast<File*>(stream->state);
    if (file->write_partial(buf, c) != (int32_t)c) {
        return false;
    }

    return true;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto file = reinterpret_cast<File*>(stream->state);
    if (file->read(buf, c) != (int32_t)c) {
        return false;
    }

    return true;
}

pb_ostream_t pb_ostream_from_file(File *file) {
    return { &write_callback, (void *)file, SIZE_MAX, 0 };
}

pb_istream_t pb_istream_from_file(File *file, size_t size) {
    return { &read_callback, (void *)file, size, 0 };
}

int32_t File::try_write(void const *record, pb_msgdesc_t const *fields) {
    storage_->verify_mutable();

    size_t record_size = 0;
    if (!pb_get_encoded_size(&record_size, fields, record)) {
        return 0;
    }

    record_size += pb_varint_size(record_size);

    if (write_record_header(record_size) == 0) {
        return 0;
    }

    pb_ostream_t ostream = pb_ostream_from_file(this);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        return 0;
    }

    if (write_record_tail(record_size) == 0) {
        return 0;
    }

    if (memory_.flush() <= 0) {
        // NOTE: This works because we only ever write a record to a
        // single block. So where the file pointer is, that's the bad one.
        storage_->bad_blocks_.mark_address_as_bad(tail_);
        return 0;
    }

    update();

    return record_size;
}

int32_t File::read(void *record, pb_msgdesc_t const *fields) {
    auto record_size = read_record_header();
    if (record_size == 0) {
        return 0;
    }

    pb_istream_t istream = pb_istream_from_file(this, record_size);
    if (!pb_decode_delimited(&istream, fields, record)) {
        return 0;
    }

    return record_size;
}

int32_t File::write(uint8_t const *record, size_t size) {
    auto rv = try_write(record, size);
    if (rv <= 0) {
        position_ -= size;
        bytes_in_block_ -= size;
        size_ -= size;
        records_in_block_--;
        record_--;

        // Well this sucks..
        auto block_tail_address = tail_;
        BlockTail block_tail;
        block_tail.bytes_in_block = bytes_in_block_;
        block_tail.records_in_block = records_in_block_;
        block_tail.block_tail = block_tail_address;
        tail_ = storage_->allocate(file_, tail_, block_tail);
        FK_ASSERT(tail_ != InvalidAddress);
        bytes_in_block_ = 0;
        records_in_block_ = 0;

        rv = try_write(record, size);
    }
    return rv;
}

int32_t File::write(void const *record, pb_msgdesc_t const *fields) {
    auto size_before = size_;
    auto rv = try_write(record, fields);
    if (rv <= 0) {
        auto size = size_ - size_before;
        position_ -= size;
        bytes_in_block_ -= size;
        size_ -= size;
        records_in_block_--;
        record_--;

        // Well this sucks..
        auto block_tail_address = tail_;
        BlockTail block_tail;
        block_tail.bytes_in_block = bytes_in_block_;
        block_tail.records_in_block = records_in_block_;
        block_tail.block_tail = block_tail_address;
        tail_ = storage_->allocate(file_, tail_, block_tail);
        FK_ASSERT(tail_ != InvalidAddress);
        bytes_in_block_ = 0;
        records_in_block_ = 0;

        rv = try_write(record, fields);
    }
    return rv;
}

}
