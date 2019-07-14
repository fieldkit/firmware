#include "storage.h"

#include <phylum/crc.h>
#include <phylum/blake2b.h>

namespace fk {

FK_DECLARE_LOGGER("storage");

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

uint32_t RecordHead::sign() {
    return phylum::crc32_checksum((uint8_t *)this, sizeof(uint32_t) * 2);
}

bool RecordHead::valid() {
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

uint32_t Storage::allocate(uint8_t file) {
    auto g = memory_->geometry();
    auto address = free_block_ * g.block_size;

    loginfo("[%d] allocated block #%d (0x%06x)", file, free_block_, address);

    free_block_++;
    timestamp_++;

    files_[file].tail = address + sizeof(BlockHeader);

    BlockHeader block_header;
    block_header.magic.fill();
    block_header.file = file;
    block_header.timestamp = timestamp_;
    for (auto i = 0; i < NumberOfFiles; ++i) {
        block_header.files[i] = files_[i];
    }
    block_header.fill_hash();

    if (!memory_->erase_block(address)) {
        return InvalidAddress;
    }

    if (!memory_->write(address, (uint8_t *)&block_header, sizeof(BlockHeader))) {
        return InvalidAddress;
    }

    return files_[file].tail;
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

        address = range.start * g.block_size;
    }

    FK_ASSERT(is_address_valid(address));

    // Adjust start position if we're at the start of a block.
    if (address % g.block_size == 0) {
        address += sizeof(BlockHeader);
    }

    logtrace("[%d] seeking #%d (0x%06x)", settings.file, settings.record, address);

    while (true) {
        auto record_head = RecordHead{};

        // NOTE: This needs to cross blocks.
        if (memory.read(address, (uint8_t *)&record_head, sizeof(record_head)) != sizeof(record_head)) {
            return SeekValue{ };
        }

        // Is there a valid record here?
        if (record_head.size == 0 || record_head.size == InvalidSize) {
            break;
        }

        // Is this the record they're looking for?
        if (settings.record != InvalidRecord && record_head.record == settings.record) {
            break;
        }

        // Keep track of maximum record for the file.
        if (record_head.record > record) {
            record = record_head.record + 1;
        }

        // Skip over the record head, the actual record, and the tail (hash)
        address += sizeof(RecordHead) + record_head.size + sizeof(RecordTail);

        // Update size of the data we've scanned through.
        position += record_head.size;
    }

    logtrace("[%d] seek done @ 0x%06x (%d) (%d bytes)", settings.file, address, record, position);

    return SeekValue{ address, record, position };
}

File Storage::file(uint8_t file) {
    return File{ this, file, files_[file] };
}

File::File(Storage *storage, uint8_t file, FileHeader fh) : storage_(storage), file_(file), tail_(fh.tail), record_(fh.record), size_(fh.size) {
    FK_ASSERT(file_ < NumberOfFiles);
}

File::~File() {
}

int32_t File::write(uint8_t *record, uint32_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto header = false;
    auto footer = false;
    auto available = g.remaining_in_block(tail_);
    auto wrote = 0;

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    if (!is_address_valid(tail_)) {
        tail_ = storage_->allocate(file_);
        available = g.remaining_in_block(tail_);
    }

    logtrace("[%d] write 0x%06x (%d bytes) #%d", file_, tail_, size, record_);

    while (!header || (uint32_t)wrote < size || !footer) {
        if (!is_address_valid(tail_) || (!header && available < sizeof(RecordHead)) || available == 0 || (!footer && available < sizeof(RecordTail))) {
            tail_ = storage_->allocate(file_);
            available = g.remaining_in_block(tail_);
        }

        if (!header) {
            RecordHead record_header = { 0 };
            record_header.size = size;
            record_header.record = record_++;
            record_header.crc = record_header.sign();
            if (memory.write(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return 0;
            }

            tail_ += sizeof(record_header);
            available -= sizeof(record_header);
            header = true;

            b2b.update(&header, sizeof(header));
        }
        else if ((uint32_t)wrote < size) {
            auto writing = std::min(available, size - wrote);
            FK_ASSERT(writing > 0);
            if (memory.write(tail_, (uint8_t *)record + wrote, writing) != writing) {
                return 0;
            }

            tail_ += writing;
            wrote += writing;
            available -= writing;
        }
        else {
            break;
        }
    }

    uint8_t hash[Hash::Length];
    b2b.update(record, size);
    b2b.finalize(hash, sizeof(hash));
    if (memory.write(tail_, (uint8_t *)hash, sizeof(hash)) != sizeof(hash)) {
        return 0; // NOTE: Hmmm.
    }

    tail_ += sizeof(hash);
    size_ += wrote;

    update();

    return wrote;
}

int32_t File::write(fk_data_DataRecord *record) {
    size_t size = 0;
    auto fields = fk_data_DataRecord_fields;
    if (!pb_get_encoded_size(&size, fields, record)) {
        return 0;
    }

    return 0;
}

int32_t File::seek(uint32_t record) {
    auto sv = storage_->seek(SeekSettings{ file_, record });
    if (!sv.valid()) {
        return false;
    }

    tail_ = sv.address;
    record_ = sv.record;
    size_ = sv.position;
    nrecord_ = sv.address;

    return true;
}

int32_t File::read(uint8_t *record, uint32_t size) {
    SequentialMemory memory{ storage_->memory_ };
    auto g = storage_->memory_->geometry();
    auto available = g.remaining_in_block(tail_);
    auto bytes_read = (uint32_t)0;

    while (bytes_read < size) {
        if (tail_ == nrecord_) {
            RecordHead record_header = { 0 };
            if (memory.read(tail_, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return bytes_read;
            }

            if (record_header.size == 0 || record_header.size == InvalidSize) {
                break;
            }

            tail_ += sizeof(record_header);
            available -= sizeof(record_header);
            nrecord_ = tail_ + record_header.size + sizeof(RecordTail);
        }
        else if (bytes_read < size) {
            auto reading = std::min(available, size - bytes_read);
            FK_ASSERT(reading > 0);
            if (memory.read(tail_, (uint8_t *)record + bytes_read, reading) != reading) {
                return bytes_read;
            }

            tail_ += reading;
            bytes_read += reading;
            available -= reading;
            if (available == 0) {
                available = g.remaining_in_block(tail_);
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
