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

    logtrace("opening");

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

    loginfo("formatting");

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        auto address = range.middle_block() * g.block_size;
        if (!memory_->erase_block(address)) {
            return false;
        }
        range = range.first_half();
    }

    free_block_ = 0;

    return true;
}

uint32_t Storage::allocate(uint8_t file, uint32_t overflow, uint32_t previous_tail_address) {
    auto g = memory_->geometry();
    auto address = free_block_ * g.block_size;

    FK_ASSERT(is_address_valid(address));

    logdebug("[%d] allocated block #%d (0x%06x) (%d)", file, free_block_, address, overflow);

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

    // If we don't have a starting address then fall back on a binary search.
    if (!is_address_valid(address)) {
        BlockHeader block_header;

        auto range = BlockRange{ 0, g.nblocks };
        while (!range.empty()) {
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
            }
            else {
                // Search earlier in the range for a valid block.
                range = range.first_half();
            }
        }

        // If at the start of the block, bump.
        address = range.start * g.block_size + sizeof(BlockHeader) + block_header.overflow;
    }

    FK_ASSERT(is_address_valid(address));

    logtrace("[%d] 0x%06x seeking #%d %d", settings.file, address, settings.record, sizeof(BlockHeader));

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
            logtrace("[%d] 0x%06x found record #%d", settings.file, settings.record, address);
            break;
        }

        // Keep track of maximum record for the file.
        if (record_head.record > record) {
            record = record_head.record + 1;
        }

        auto record_length = sizeof(RecordHeader) + record_head.size + sizeof(RecordTail);

        logverbose("[%d] 0x%06x seeking %4d/%4d", settings.file, address, record_length, record_head.size);

        // Skip over the record head, the actual record, and the tail (hash)
        FK_ASSERT(!g.spans_block(address, record_length));
        address += record_length;

        // Update size of the data we've scanned through.
        position += record_head.size;
    }

    logtrace("[%d] 0x%06x seek done @ (%d) (%d bytes)", settings.file, address, record, position);

    return SeekValue{ address, record, position };
}

File Storage::file(uint8_t file) {
    return File{ this, file, files_[file] };
}

File::File(Storage *storage, uint8_t file, FileHeader fh)
    : storage_(storage), file_(file), tail_(fh.tail), record_(fh.record), size_(fh.size) {
    FK_ASSERT(file_ < NumberOfFiles);
}

File::~File() {
}

size_t File::write(uint8_t *record, size_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto header = false;
    auto footer = false;
    auto wrote = (size_t)0;
    auto left_in_block = (g.remaining_in_block(tail_) - sizeof(BlockTail));
    auto total_required = sizeof(RecordHeader) + size + sizeof(RecordTail);

    if (!is_address_valid(tail_) || total_required > left_in_block) {
        if (is_address_valid(tail_)) {
            tail_ += left_in_block;
        }
        tail_ = storage_->allocate(file_, 0, tail_);
        left_in_block = g.remaining_in_block(tail_) - sizeof(BlockTail);
    }

    logtrace("[%d] 0x%06x BEGIN write (%d bytes) #%d (%d w/ overhead)", file_, tail_, size, record_,
             sizeof(RecordHeader) + sizeof(RecordTail) + size);

    hash_.reset(Hash::Length);

    while (!header || (uint32_t)wrote < size || !footer) {
        if (!is_address_valid(tail_) || left_in_block == 0 ||
            (!header && !footer && left_in_block < sizeof(RecordHeader)) ||
            (!footer && (uint32_t)wrote == size && left_in_block < sizeof(RecordTail))) {

            auto overflow = size - wrote;
            if (!footer) {
                overflow += sizeof(RecordTail);
            }
            tail_ = storage_->allocate(file_, overflow, tail_ + left_in_block);
            left_in_block = g.remaining_in_block(tail_) - sizeof(BlockTail);
        }

        if (!header) {
            RecordHeader record_header;
            record_header.size = size;
            record_header.record = record_++;
            record_header.crc = record_header.sign();

            logverbose("[%d] 0x%06x write header (lib = %d) (%d bytes)", file_, tail_, left_in_block, size);

            FK_ASSERT(wrote == 0);

            if (memory.write(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return 0;
            }

            hash_.update(&header, sizeof(header));

            tail_ += sizeof(record_header);
            left_in_block -= sizeof(record_header);
            header = true;
        }
        else if (wrote < size) {
            auto writing = std::min<size_t>(left_in_block, size - wrote);
            logverbose("[%d] 0x%06x write data (%d bytes) (%d lib) (%d to write)",
                     file_, tail_, writing, left_in_block, size - wrote);

            FK_ASSERT(writing > 0);

            if (memory.write(tail_, (uint8_t *)record + wrote, writing) != writing) {
                return 0;
            }

            tail_ += writing;
            wrote += writing;
            left_in_block -= writing;
        }
        else if (wrote == size && !footer) {
            RecordTail record_tail;
            hash_.update(record, size);
            hash_.finalize(record_tail.hash.hash, sizeof(record_tail.hash.hash));
            if (memory.write(tail_, (uint8_t *)&record_tail, sizeof(RecordTail)) != sizeof(RecordTail)) {
                return 0;
            }

            logverbose("[%d] 0x%06x write footer (lib = %d)", file_, tail_, left_in_block);

            tail_ += sizeof(RecordTail);
            left_in_block -= sizeof(RecordTail);
            size_ += wrote;
            footer = true;
        }
        else {
            break;
        }
    }

    if (left_in_block == 0) {
        tail_ = storage_->allocate(file_, 0, tail_);
        left_in_block = g.remaining_in_block(tail_);
    }

    update();

    return wrote;
}

size_t File::write(fk_data_DataRecord *record) {
    size_t size = 0;
    auto fields = fk_data_DataRecord_fields;
    if (!pb_get_encoded_size(&size, fields, record)) {
        return 0;
    }

    return 0;
}

size_t File::seek(uint32_t record) {
    auto sv = storage_->seek(SeekSettings{ file_, record });
    if (!sv.valid()) {
        return false;
    }

    tail_ = sv.address;
    record_ = sv.record;
    size_ = sv.position;
    record_remaining_ = 0;

    return true;
}

size_t File::read(uint8_t *record, size_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
    auto bytes_read = (size_t)0;
    auto minimum_record_size = sizeof(RecordHeader) + sizeof(RecordTail);

    logtrace("[%d] 0x%06x BEGIN read (%d bytes) (rr = %d) (lib = %d)", file_, tail_, size, record_remaining_, left_in_block);

    while (bytes_read < size) {
        if (left_in_block < minimum_record_size) {
            tail_ += left_in_block;

            BlockTail block_tail;
            if (memory.read(tail_, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
                break;
            }

            logverbose("[%d] 0x%06x btail (0x%06x)", file_, tail_, block_tail.linked);

            tail_ += sizeof(BlockTail);

            tail_ = block_tail.linked;

            BlockHeader block_header;
            if (memory.read(tail_, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
                break;
            }

            FK_ASSERT(block_header.verify_hash());

            FK_ASSERT(block_header.file == file_);

            tail_ += sizeof(BlockHeader);
            left_in_block = g.remaining_in_block(tail_) - sizeof(BlockTail);
        }
        else if (record_remaining_ == 0) {
            RecordHeader record_header;
            if (memory.read(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return bytes_read;
            }

            if (!record_header.valid()) {
                tail_ += left_in_block;
                left_in_block = (uint32_t)(g.remaining_in_block(tail_) - sizeof(BlockTail));
                continue;
            }

            hash_.reset(Hash::Length);
            hash_.update(&record_header, sizeof(RecordHeader));

            record_remaining_ = record_header.size;

            logverbose("[%d] 0x%06x record header (%d bytes) #%d", file_, tail_, record_remaining_, record_header.record);

            tail_ += sizeof(record_header);
            left_in_block -= sizeof(record_header);
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

            if (record_remaining_ == 0) {
                logverbose("[%d] 0x%06x end of record", file_, tail_);
                tail_ += sizeof(RecordTail);
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

}
