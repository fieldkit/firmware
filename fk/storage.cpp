#include "storage.h"

namespace fk {

FK_DECLARE_LOGGER("storage");

#define logverbose(f, ...)

constexpr char BlockMagic::MagicKey[];

uint32_t hash_block(void *ptr, size_t size, Hash &hash) {
    BLAKE2b b2b;
    b2b.reset(Hash::Length);
    b2b.update(ptr, size);
    b2b.finalize(&hash.hash, Hash::Length);
    return 32;
}

struct BlockRange {
    uint32_t start;
    uint32_t nblocks;

    uint32_t middle_block() const {
        return start + (nblocks / 2);
    }

    BlockRange first_half() const {
        return { start, (nblocks / 2) };
    }

    BlockRange second_half() const {
        return { start + (nblocks / 2), (nblocks / 2) };
    }

    bool empty() const {
        return nblocks == 0;
    }
};

SeekSettings SeekSettings::end_of(uint8_t file) {
    return SeekSettings{ file, LastRecord };
}

uint32_t RecordHeader::sign() {
    return phylum::crc32_checksum((uint8_t *)this, sizeof(uint32_t) * 2);
}

bool RecordHeader::valid() {
    return sign() == crc;
}

Storage::Storage(DataMemory *memory) : memory_(memory) {
}

bool Storage::begin() {
    auto g = memory_->geometry();

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    // Look for the first available free block.
    auto had_valid_blocks = false;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (!memory_->read(address, (uint8_t *)&block_header, sizeof(block_header))) {
            return false;
        }

        if (block_header.magic.valid()) {
            logtrace("[%d] found valid block (0x%06x)", block_header.file, address);

            FK_ASSERT(block_header.verify_hash());

            for (auto i = 0; i < NumberOfFiles; ++i) {
                files_[i] = block_header.files[i];
            }

            range = range.second_half();
            had_valid_blocks = true;
        }
        else {
            range = range.first_half();
        }
    }

    if (!had_valid_blocks) {
        return false;
    }

    free_block_ = range.start;

    // Make sure our header records are fully up to date by seeking to the end
    // of each file.
    for (auto file = (uint8_t)0; file < NumberOfFiles; ++file) {
        if (is_address_valid(files_[file].tail)) {
            auto sv = seek(SeekSettings::end_of(file));
            if (!sv.valid()) {
                return false;
            }
            files_[file].tail = sv.address;
            files_[file].size = sv.position;
            files_[file].record = sv.record;
        }
    }

    logtrace("opened (block = %d)", free_block_);

    return true;
}

bool Storage::clear() {
    auto g = memory_->geometry();

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        auto address = range.middle_block() * g.block_size;
        if (!memory_->erase_block(address)) {
            return false;
        }
        range = range.first_half();
    }

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    free_block_ = 0;

    return true;
}

uint32_t Storage::allocate(uint8_t file, uint32_t overflow, uint32_t previous_tail_address) {
    auto g = memory_->geometry();
    auto address = free_block_ * g.block_size;

    FK_ASSERT(is_address_valid(address));

    logdebug("[%d] allocated block #%d (0x%06x) (%d) (%d bytes)", file, free_block_, address, overflow, files_[file].size);

    free_block_++;
    timestamp_++;

    auto after_header = address + sizeof(BlockHeader);

    // Do this before writing header so the new value gets written.
    files_[file].tail = after_header;

    BlockHeader block_header;
    block_header.magic.fill();
    block_header.file = file;
    block_header.timestamp = timestamp_;
    block_header.overflow = overflow;
    for (auto i = 0; i < NumberOfFiles; ++i) {
        block_header.files[i] = files_[i];
    }
    block_header.fill_hash();

    // Erase new block and write header.
    if (!memory_->erase_block(address)) {
        return InvalidAddress;
    }

    if (memory_->write(address, (uint8_t *)&block_header, sizeof(BlockHeader)) != sizeof(BlockHeader)) {
        return InvalidAddress;
    }

    // We have a good new block, so link the previous block to the new one.
    if (is_address_valid(previous_tail_address)) {
        BlockTail block_tail;
        block_tail.linked = address;
        block_tail.fill_hash();
        if (memory_->write(previous_tail_address, (uint8_t *)&block_tail, sizeof(BlockTail)) != sizeof(BlockTail)) {
            return 0;
        }

        logtrace("[%d] 0x%06x btail", file, previous_tail_address);
    }

    return after_header;
}

SeekValue Storage::seek(SeekSettings settings) {
    SequentialMemory memory{ memory_ };
    auto g = memory_->geometry();
    auto record = (uint32_t)0;
    auto position = (uint32_t)0;
    auto address = InvalidAddress;

    logtrace("[%d] seeking #%d", settings.file, settings.record);

    // Binary search for the block to start with.
    BlockHeader file_block_header;
    file_block_header.timestamp = 0;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (!memory_->read(address, (uint8_t *)&block_header, sizeof(block_header))) {
            return { };
        }

        if (block_header.magic.valid()) {
            auto &bfh = block_header.files[settings.file];
            if (settings.record != InvalidRecord) {
                if (bfh.record > settings.record) {
                    range = range.first_half();
                }
                else {
                    range = range.second_half();
                }
            }
            else {
                range = range.second_half();
            }

            memcpy(&file_block_header, &block_header, sizeof(BlockHeader));
        }
        else {
            // Search earlier in the range for a valid block.
            range = range.first_half();
        }
    }

    // If at the start of the block, bump.
    auto &fh = file_block_header.files[settings.file];
    address = fh.tail;
    position = fh.size;

    FK_ASSERT(is_address_valid(address));

    logtrace("[%d] tail = 0x%06x (block %d)", settings.file, fh.tail, fh.tail / g.block_size);
    logtrace("[%d] 0x%06x seeking #%d (%d) from #%d (bsz = %d bytes)", settings.file, address, settings.record, position, fh.tail / g.block_size, fh.size);

    while (true) {
        auto record_head = RecordHeader{};

        // If at the start of the block, bump.
        if (g.start_of_block(address)) {
            address += sizeof(BlockHeader);
        }

        // NOTE: This needs to cross blocks.
        if (memory.read(address, (uint8_t *)&record_head, sizeof(record_head)) != sizeof(record_head)) {
            return SeekValue{ };
        }

        // Is there a valid record here?
        if (record_head.size == 0 || record_head.size == InvalidSize || !record_head.valid()) {
            logtrace("[%d] 0x%06x invalid head", settings.file, address);
            break;
        }

        // Is this the record they're looking for?
        if (settings.record != InvalidRecord && record_head.record == settings.record) {
            logtrace("[%d] 0x%06x found record #%d", settings.file, address, settings.record);
            break;
        }

        // Keep track of maximum record for the file.
        if (record_head.record >= record) {
            record = record_head.record + 1;
        }

        auto record_length = sizeof(RecordHeader) + record_head.size + sizeof(RecordTail);

        logverbose("[%d] 0x%06x seeking %4d/%4d (#%d)", settings.file, address, record_length, record_head.size, record_head.record);

        // Skip over the record head, the actual record, and the tail (hash)
        FK_ASSERT(!g.spans_block(address, record_length));
        address += record_length;

        // Update size of the data we've scanned through.
        position += record_head.size;
    }

    logdebug("[%d] 0x%06x seek done @ (%d) (%d bytes) (%d in block)", settings.file, address, record, position, position - fh.size);

    return SeekValue{ address, record, position };
}

File Storage::file(uint8_t file) {
    return File{ this, file, files_[file] };
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

    tail_ += size;

    return size;
}

size_t File::write_record_tail(size_t size) {
    SequentialMemory memory{ storage_->memory_ };

    RecordTail record_tail;
    hash_.finalize(record_tail.hash.hash, sizeof(record_tail.hash.hash));
    if (memory.write(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
        return 0;
    }

    logverbose("[%d] 0x%06x write footer", file_, tail_);

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

            if (record_header.valid()) {
                hash_.reset(Hash::Length);
                hash_.update(&record_header, sizeof(RecordHeader));

                record_remaining_ = record_header.size;

                logverbose("[%d] 0x%06x record header (%d bytes) #%d", file_, tail_, record_remaining_, record_header.record);

                tail_ += sizeof(record_header);

                return record_remaining_;
            }

            tail_ += left_in_block;
            left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
        }
    }

    return 0;
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
    FK_ASSERT(memcmp(hash.hash, record_tail.hash.hash, Hash::Length) == 0);

    tail_ += sizeof(RecordTail);

    return sizeof(RecordTail);
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

void File::update() {
    auto &fh = storage_->files_[file_];
    fh.tail = tail_;
    fh.size = size_;
    fh.record = record_;
}

typedef struct pb_file_t {
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
